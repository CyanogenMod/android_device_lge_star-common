/*
 * Copyright (C) 2012 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.android.internal.telephony;

import static com.android.internal.telephony.RILConstants.*;

import android.content.Context;
import android.os.AsyncResult;
import android.os.Message;
import android.os.Parcel;

import android.telephony.PhoneNumberUtils;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;

/* NITZ stuffs */
import android.os.SystemProperties;
import java.util.Date;
import java.text.SimpleDateFormat;

public class LGEInfineon extends RIL implements CommandsInterface {

    public LGEInfineon(Context context, int networkMode, int cdmaSubscription) {
        super(context, networkMode, cdmaSubscription);
        PhoneStateListener mPhoneStateListener = new PhoneStateListener() {
            @Override
            public void onCallStateChanged(int state, String incomingNumber) {
                /* Higher state wins, unless going back to idle */
                if (state == TelephonyManager.CALL_STATE_IDLE || state > mCallState)
                    mCallState = state;
            }
        };

        // register for phone state notifications.
        ((TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE))
            .listen(mPhoneStateListener,
                    PhoneStateListener.LISTEN_CALL_STATE);
    }

    protected int mCallState = TelephonyManager.CALL_STATE_IDLE;

    private int RIL_REQUEST_HANG_UP_CALL = 182;

    @Override
    public void
    hangupWaitingOrBackground (Message result) {
        RILRequest rr = RILRequest.obtain(mCallState == TelephonyManager.CALL_STATE_OFFHOOK ?
                                        RIL_REQUEST_HANGUP_WAITING_OR_BACKGROUND :
                                        RIL_REQUEST_HANG_UP_CALL,
                                        result);

        if (RILJ_LOGD) riljLog(rr.serialString() + "> " + requestToString(rr.mRequest));

        send(rr);
    }

    private static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i+1), 16));
        }
        return data;
    }

    static final int RIL_UNSOL_LGE_SIM_STATE_CHANGED = 1060;
    static final int RIL_UNSOL_LGE_SIM_STATE_CHANGED_NEW = 1061;

    @Override
    protected void
    processUnsolicited (Parcel p) {
        Object ret;
        int dataPosition = p.dataPosition(); // save off position within the Parcel
        int response = p.readInt();

        switch(response) {
            case RIL_UNSOL_ON_USSD: ret =  responseStrings(p); break;
            case 1080: ret =  responseVoid(p); break; // RIL_UNSOL_LGE_FACTORY_READY
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED:
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED_NEW: ret =  responseVoid(p); break;
            case RIL_UNSOL_NITZ_TIME_RECEIVED: ret =  responseNitz(p); break;
            default:
                // Rewind the Parcel
                p.setDataPosition(dataPosition);

                // Forward responses that we are not overriding to the super class
                super.processUnsolicited(p);
                return;
        }
        switch(response) {
            case RIL_UNSOL_ON_USSD:
                String[] resp = (String[])ret;

                if (resp.length < 2) {
                    resp = new String[2];
                    resp[0] = ((String[])ret)[0];
                    resp[1] = null;
                }
                if (resp[1].length()%2 == 0 && resp[1].matches("[0-9A-F]+")) {
                    try { 
                        resp[1] = new String(hexStringToByteArray(resp[1]), "UTF-16");
                    } catch (java.io.UnsupportedEncodingException uex) { 
                        // encoding not supported, should never get here 
                    } catch (java.io.IOException iox) { 
                        // you will get here if the original sequence wasn't UTF-8 or ASCII 
                    } 
                }
                if (RILJ_LOGD) unsljLogMore(response, resp[0]);
                if (mUSSDRegistrant != null) {
                    mUSSDRegistrant.notifyRegistrant(
                        new AsyncResult (null, resp, null));
                }
                break;
            case 1080: // RIL_UNSOL_LGE_FACTORY_READY (NG)
                /* Adjust request IDs */
                RIL_REQUEST_HANG_UP_CALL = 206;
                break;
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED:
            case RIL_UNSOL_LGE_SIM_STATE_CHANGED_NEW:
                if (RILJ_LOGD) unsljLog(response);

                if (mIccStatusChangedRegistrants != null) {
                    mIccStatusChangedRegistrants.notifyRegistrants();
                }
                break;
            case RIL_UNSOL_NITZ_TIME_RECEIVED:
                if (RILJ_LOGD) unsljLogRet(response, ret);

                // has bonus long containing milliseconds since boot that the NITZ
                // time was received
                long nitzReceiveTime = p.readLong();

                Object[] result = new Object[2];

                result[0] = ret;
                result[1] = Long.valueOf(nitzReceiveTime);

                boolean ignoreNitz = SystemProperties.getBoolean(
                        TelephonyProperties.PROPERTY_IGNORE_NITZ, false);

                if (ignoreNitz) {
                    if (RILJ_LOGD) riljLog("ignoring UNSOL_NITZ_TIME_RECEIVED");
                } else {
                    if (mNITZTimeRegistrant != null) {

                        mNITZTimeRegistrant
                            .notifyRegistrant(new AsyncResult (null, result, null));
                    } else {
                        // in case NITZ time registrant isnt registered yet
                        mLastNITZTimeInfo = result;
                    }
                }
            break;
        }

    }

    private Object
    responseNitz(Parcel p) {
        int tzoffset, dst=0;
        String parceldata, parcelextra;
        String response;
        SimpleDateFormat dateFormatter;
        SimpleDateFormat dateParser;

        /* Get the actual date string */
        parceldata = p.readString();

        /* Break 12/11/6,19:0:15+44,1 into...
         * parcelitem[0] (date), hourbreak[0] (time), 
         * hourbreak[1] (tz offset), parcelitem[2] (dst) */

        String [] parcelitem = parceldata.split(",");
        String [] hourbreak = parcelitem[1].split("[+\\-]");
        parceldata = parcelitem[0] + "," + hourbreak[0]; // assuming there is always one comma at least
        tzoffset = Integer.parseInt(hourbreak[1]); // TZ diff in quarter-hours
        if (parcelitem[1].contains("-")) tzoffset *= -1;

        parcelextra = (parcelitem.length > 2 ? parcelitem[2] : "0");
        dst = Integer.parseInt(parcelextra);

        /* WTH... Date may come with 4 digits in the year, reduce to 2 */
        try {
            dateFormatter = new SimpleDateFormat("yy/MM/dd,HH:mm:ss");
            dateParser = new SimpleDateFormat("yy/MM/dd,HH:mm:ss");

            /* Directly calculate UTC time using DST Offset */
            int offset = tzoffset*15*60*1000;	// DST corrected
            long when = dateParser.parse(parceldata).getTime() - offset;
            Date d = new Date(when);
            response = dateFormatter.format(d);

        } catch (java.text.ParseException tpe) {
            riljLog("NITZ TZ conversion failed: " + tpe);
            response = parceldata;
        }

        /* Append the timezone */
        response = response + ((tzoffset < 0) ? "" : "+") + tzoffset;
        /* Add DST */
        response = response + "," + dst;

        return response;
    }

}

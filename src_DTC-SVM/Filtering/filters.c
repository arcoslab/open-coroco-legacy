/*
 *This file is part of the open-coroco project.
 *
 *  Copyright (C) 2013  Sebastian Chinchilla Gutierrez <tumacher@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
float isA_moving_average_filter(float isA)
{
    static float isA_49=0.0f;
    static float isA_48=0.0f;
    static float isA_47=0.0f;
    static float isA_46=0.0f;
    static float isA_45=0.0f;
    static float isA_44=0.0f;
    static float isA_43=0.0f;
    static float isA_42=0.0f;
    static float isA_41=0.0f;
    static float isA_40=0.0f;
    static float isA_39=0.0f;
    static float isA_38=0.0f;
    static float isA_37=0.0f;
    static float isA_36=0.0f;
    static float isA_35=0.0f;
    static float isA_34=0.0f;
    static float isA_33=0.0f;
    static float isA_32=0.0f;
    static float isA_31=0.0f;
    static float isA_30=0.0f;
    static float isA_29=0.0f;
    static float isA_28=0.0f;
    static float isA_27=0.0f;
    static float isA_26=0.0f;
    static float isA_25=0.0f;
    static float isA_24=0.0f;
    static float isA_23=0.0f;
    static float isA_22=0.0f;
    static float isA_21=0.0f;
    static float isA_20=0.0f;
    static float isA_19=0.0f;
    static float isA_18=0.0f;
    static float isA_17=0.0f;
    static float isA_16=0.0f;
    static float isA_15=0.0f;
    static float isA_14=0.0f;
    static float isA_13=0.0f;
    static float isA_12=0.0f;
    static float isA_11=0.0f;
    static float isA_10=0.0f;
    static float isA_09=0.0f;
    static float isA_08=0.0f;
    static float isA_07=0.0f;
    static float isA_06=0.0f;
    static float isA_05=0.0f;
    static float isA_04=0.0f;
    static float isA_03=0.0f;
    static float isA_02=0.0f;
    static float isA_01=0.0f;
    static float isA_00=0.0f;
    static int   k=0;
    float filtered_isA=0.0f;

    if (k==0)
    {
    isA_49=isA;
    isA_48=isA;
    isA_47=isA;
    isA_46=isA;
    isA_45=isA;
    isA_44=isA;
    isA_43=isA;
    isA_42=isA;
    isA_41=isA;
    isA_40=isA;
    isA_39=isA;
    isA_38=isA;
    isA_37=isA;
    isA_36=isA;
    isA_35=isA;
    isA_34=isA;
    isA_33=isA;
    isA_32=isA;
    isA_31=isA;
    isA_30=isA;
    isA_29=isA;
    isA_28=isA;
    isA_27=isA;
    isA_26=isA;
    isA_25=isA;
    isA_24=isA;
    isA_23=isA;
    isA_22=isA;
    isA_21=isA;
    isA_20=isA;
    isA_19=isA;
    isA_18=isA;
    isA_17=isA;
    isA_16=isA;
    isA_15=isA;
    isA_14=isA;
    isA_13=isA;
    isA_12=isA;
    isA_11=isA;
    isA_10=isA;
    isA_09=isA;
    isA_08=isA;
    isA_07=isA;
    isA_06=isA;
    isA_05=isA;
    isA_04=isA;
    isA_03=isA;
    isA_02=isA;
    isA_01=isA;
    isA_00=isA;
        k         = 1;
    }

        isA_49=isA_48;
        isA_48=isA_47;
        isA_47=isA_46;
        isA_46=isA_45;
        isA_45=isA_44;
        isA_44=isA_43;
        isA_43=isA_42;
        isA_42=isA_41;
        isA_41=isA_40;
        isA_40=isA_39;
        isA_39=isA_38;
        isA_38=isA_37;
        isA_37=isA_36;
        isA_36=isA_35;
        isA_35=isA_34;
        isA_34=isA_33;
        isA_33=isA_32;
        isA_32=isA_31;
        isA_31=isA_30;
        isA_30=isA_29;
        isA_29=isA_28;
        isA_28=isA_27;
        isA_27=isA_26;
        isA_26=isA_25;
        isA_25=isA_24;
        isA_24=isA_23;
        isA_23=isA_22;
        isA_22=isA_21;
        isA_21=isA_20;
        isA_20=isA_19;
        isA_19=isA_18;
        isA_18=isA_17;
        isA_17=isA_16;
        isA_16=isA_15;
        isA_15=isA_14;
        isA_14=isA_13;
        isA_13=isA_12;
        isA_12=isA_11;
        isA_11=isA_10;
        isA_10=isA_09;
        isA_09=isA_08;
        isA_08=isA_07;
        isA_07=isA_06;
        isA_06=isA_05;
        isA_05=isA_04;
        isA_04=isA_03;
        isA_03=isA_02;
        isA_02=isA_01;
        isA_01=isA_00;
        isA_00=isA;

    filtered_isA = (isA_00+isA_01+isA_02+isA_03+isA_04+isA_05+isA_06+isA_07+isA_08+isA_09+
                    isA_10+isA_11+isA_12+isA_13+isA_14+isA_15+isA_16+isA_17+isA_18+isA_19+
                    isA_20+isA_21+isA_22+isA_23+isA_24+isA_25+isA_26+isA_27+isA_28+isA_29+
                    isA_30+isA_31+isA_32+isA_33+isA_34+isA_35+isA_36+isA_37+isA_38+isA_39+
                    isA_40+isA_41+isA_42+isA_43+isA_44+isA_45+isA_46+isA_47+isA_48+isA_49)/50.0f;
    
    return filtered_isA;
}



float isB_moving_average_filter(float isB)
{
    static float isB_49=0.0f;
    static float isB_48=0.0f;
    static float isB_47=0.0f;
    static float isB_46=0.0f;
    static float isB_45=0.0f;
    static float isB_44=0.0f;
    static float isB_43=0.0f;
    static float isB_42=0.0f;
    static float isB_41=0.0f;
    static float isB_40=0.0f;
    static float isB_39=0.0f;
    static float isB_38=0.0f;
    static float isB_37=0.0f;
    static float isB_36=0.0f;
    static float isB_35=0.0f;
    static float isB_34=0.0f;
    static float isB_33=0.0f;
    static float isB_32=0.0f;
    static float isB_31=0.0f;
    static float isB_30=0.0f;
    static float isB_29=0.0f;
    static float isB_28=0.0f;
    static float isB_27=0.0f;
    static float isB_26=0.0f;
    static float isB_25=0.0f;
    static float isB_24=0.0f;
    static float isB_23=0.0f;
    static float isB_22=0.0f;
    static float isB_21=0.0f;
    static float isB_20=0.0f;
    static float isB_19=0.0f;
    static float isB_18=0.0f;
    static float isB_17=0.0f;
    static float isB_16=0.0f;
    static float isB_15=0.0f;
    static float isB_14=0.0f;
    static float isB_13=0.0f;
    static float isB_12=0.0f;
    static float isB_11=0.0f;
    static float isB_10=0.0f;
    static float isB_09=0.0f;
    static float isB_08=0.0f;
    static float isB_07=0.0f;
    static float isB_06=0.0f;
    static float isB_05=0.0f;
    static float isB_04=0.0f;
    static float isB_03=0.0f;
    static float isB_02=0.0f;
    static float isB_01=0.0f;
    static float isB_00=0.0f;
    static int   k=0;
    float filtered_isB=0.0f;

    if (k==0)
    {
    isB_49=isB;
    isB_48=isB;
    isB_47=isB;
    isB_46=isB;
    isB_45=isB;
    isB_44=isB;
    isB_43=isB;
    isB_42=isB;
    isB_41=isB;
    isB_40=isB;
    isB_39=isB;
    isB_38=isB;
    isB_37=isB;
    isB_36=isB;
    isB_35=isB;
    isB_34=isB;
    isB_33=isB;
    isB_32=isB;
    isB_31=isB;
    isB_30=isB;
    isB_29=isB;
    isB_28=isB;
    isB_27=isB;
    isB_26=isB;
    isB_25=isB;
    isB_24=isB;
    isB_23=isB;
    isB_22=isB;
    isB_21=isB;
    isB_20=isB;
    isB_19=isB;
    isB_18=isB;
    isB_17=isB;
    isB_16=isB;
    isB_15=isB;
    isB_14=isB;
    isB_13=isB;
    isB_12=isB;
    isB_11=isB;
    isB_10=isB;
    isB_09=isB;
    isB_08=isB;
    isB_07=isB;
    isB_06=isB;
    isB_05=isB;
    isB_04=isB;
    isB_03=isB;
    isB_02=isB;
    isB_01=isB;
    isB_00=isB;
        k         = 1;
    }

        isB_49=isB_48;
        isB_48=isB_47;
        isB_47=isB_46;
        isB_46=isB_45;
        isB_45=isB_44;
        isB_44=isB_43;
        isB_43=isB_42;
        isB_42=isB_41;
        isB_41=isB_40;
        isB_40=isB_39;
        isB_39=isB_38;
        isB_38=isB_37;
        isB_37=isB_36;
        isB_36=isB_35;
        isB_35=isB_34;
        isB_34=isB_33;
        isB_33=isB_32;
        isB_32=isB_31;
        isB_31=isB_30;
        isB_30=isB_29;
        isB_29=isB_28;
        isB_28=isB_27;
        isB_27=isB_26;
        isB_26=isB_25;
        isB_25=isB_24;
        isB_24=isB_23;
        isB_23=isB_22;
        isB_22=isB_21;
        isB_21=isB_20;
        isB_20=isB_19;
        isB_19=isB_18;
        isB_18=isB_17;
        isB_17=isB_16;
        isB_16=isB_15;
        isB_15=isB_14;
        isB_14=isB_13;
        isB_13=isB_12;
        isB_12=isB_11;
        isB_11=isB_10;
        isB_10=isB_09;
        isB_09=isB_08;
        isB_08=isB_07;
        isB_07=isB_06;
        isB_06=isB_05;
        isB_05=isB_04;
        isB_04=isB_03;
        isB_03=isB_02;
        isB_02=isB_01;
        isB_01=isB_00;
        isB_00=isB;

    filtered_isB = (isB_00+isB_01+isB_02+isB_03+isB_04+isB_05+isB_06+isB_07+isB_08+isB_09+
                    isB_10+isB_11+isB_12+isB_13+isB_14+isB_15+isB_16+isB_17+isB_18+isB_19+
                    isB_20+isB_21+isB_22+isB_23+isB_24+isB_25+isB_26+isB_27+isB_28+isB_29+
                    isB_30+isB_31+isB_32+isB_33+isB_34+isB_35+isB_36+isB_37+isB_38+isB_39+
                    isB_40+isB_41+isB_42+isB_43+isB_44+isB_45+isB_46+isB_47+isB_48+isB_49)/50.0f;
    
    return filtered_isB;
}
*/


float isA_moving_average_filter(float isA)
{

    static float isA_49=0.0f;
    static float isA_48=0.0f;
    static float isA_47=0.0f;
    static float isA_46=0.0f;
    static float isA_45=0.0f;
    static float isA_44=0.0f;
    static float isA_43=0.0f;
    static float isA_42=0.0f;
    static float isA_41=0.0f;
    static float isA_40=0.0f;
    static float isA_39=0.0f;
    static float isA_38=0.0f;
    static float isA_37=0.0f;
    static float isA_36=0.0f;
    static float isA_35=0.0f;
    static float isA_34=0.0f;
    static float isA_33=0.0f;
    static float isA_32=0.0f;
    static float isA_31=0.0f;
    static float isA_30=0.0f;
    static float isA_29=0.0f;
    static float isA_28=0.0f;
    static float isA_27=0.0f;
    static float isA_26=0.0f;
    static float isA_25=0.0f;
    static float isA_24=0.0f;
    static float isA_23=0.0f;
    static float isA_22=0.0f;
    static float isA_21=0.0f;
    static float isA_20=0.0f;
    static float isA_19=0.0f;
    static float isA_18=0.0f;
    static float isA_17=0.0f;
    static float isA_16=0.0f;
    static float isA_15=0.0f;
    static float isA_14=0.0f;
    static float isA_13=0.0f;
    static float isA_12=0.0f;
    static float isA_11=0.0f;
    static float isA_10=0.0f;
    static float isA_09=0.0f;
    static float isA_08=0.0f;
    static float isA_07=0.0f;
    static float isA_06=0.0f;
    static float isA_05=0.0f;
    static float isA_04=0.0f;
    static float isA_03=0.0f;
    static float isA_02=0.0f;
    static float isA_01=0.0f;
    static float isA_00=0.0f;
    static int   k=0;
    float filtered_isA=0.0f;

    if (k==0)
    {

    isA_49=isA;
    isA_48=isA;
    isA_47=isA;
    isA_46=isA;
    isA_45=isA;
    isA_44=isA;
    isA_43=isA;
    isA_42=isA;
    isA_41=isA;
    isA_40=isA;
    isA_39=isA;
    isA_38=isA;
    isA_37=isA;
    isA_36=isA;
    isA_35=isA;
    isA_34=isA;
    isA_33=isA;
    isA_32=isA;
    isA_31=isA;
    isA_30=isA;
    isA_29=isA;
    isA_28=isA;
    isA_27=isA;
    isA_26=isA;
    isA_25=isA;
    isA_24=isA;
    isA_23=isA;
    isA_22=isA;
    isA_21=isA;
    isA_20=isA;
    isA_19=isA;
    isA_18=isA;
    isA_17=isA;
    isA_16=isA;
    isA_15=isA;
    isA_14=isA;
    isA_13=isA;
    isA_12=isA;
    isA_11=isA;
    isA_10=isA;
    isA_09=isA;
    isA_08=isA;
    isA_07=isA;
    isA_06=isA;
    isA_05=isA;
    isA_04=isA;
    isA_03=isA;
    isA_02=isA;
    isA_01=isA;
    isA_00=isA;
    k         = 1;
    }

        isA_49=isA_48;
        isA_48=isA_47;
        isA_47=isA_46;
        isA_46=isA_45;
        isA_45=isA_44;
        isA_44=isA_43;
        isA_43=isA_42;
        isA_42=isA_41;
        isA_41=isA_40;
        isA_40=isA_39;
        isA_39=isA_38;
        isA_38=isA_37;
        isA_37=isA_36;
        isA_36=isA_35;
        isA_35=isA_34;
        isA_34=isA_33;
        isA_33=isA_32;
        isA_32=isA_31;
        isA_31=isA_30;
        isA_30=isA_29;
        isA_29=isA_28;
        isA_28=isA_27;
        isA_27=isA_26;
        isA_26=isA_25;
        isA_25=isA_24;
        isA_24=isA_23;
        isA_23=isA_22;
        isA_22=isA_21;
        isA_21=isA_20;
        isA_20=isA_19;
        isA_19=isA_18;
        isA_18=isA_17;
        isA_17=isA_16;
        isA_16=isA_15;
        isA_15=isA_14;
        isA_14=isA_13;
        isA_13=isA_12;
        isA_12=isA_11;
        isA_11=isA_10;
        isA_10=isA_09;
        isA_09=isA_08;
        isA_08=isA_07;
        isA_07=isA_06;
        isA_06=isA_05;
        isA_05=isA_04;
        isA_04=isA_03;
        isA_03=isA_02;
        isA_02=isA_01;
        isA_01=isA_00;
        isA_00=isA;

    filtered_isA = (isA_00+isA_01+isA_02+isA_03+isA_04+isA_05+isA_06+isA_07+isA_08+isA_09+
                    isA_10+isA_11+isA_12+isA_13+isA_14+isA_15+isA_16+isA_17+isA_18+isA_19+
                    isA_20+isA_21+isA_22+isA_23+isA_24+isA_25+isA_26+isA_27+isA_28+isA_29+
                    isA_30+isA_31+isA_32+isA_33+isA_34+isA_35+isA_36+isA_37+isA_38+isA_39+
                    isA_40+isA_41+isA_42+isA_43+isA_44+isA_45+isA_46+isA_47+isA_48+isA_49)/50.0f;
    
    return filtered_isA;
}



float isB_moving_average_filter(float isB)
{

    static float isB_49=0.0f;
    static float isB_48=0.0f;
    static float isB_47=0.0f;
    static float isB_46=0.0f;
    static float isB_45=0.0f;
    static float isB_44=0.0f;
    static float isB_43=0.0f;
    static float isB_42=0.0f;
    static float isB_41=0.0f;
    static float isB_40=0.0f;
    static float isB_39=0.0f;
    static float isB_38=0.0f;
    static float isB_37=0.0f;
    static float isB_36=0.0f;
    static float isB_35=0.0f;
    static float isB_34=0.0f;
    static float isB_33=0.0f;
    static float isB_32=0.0f;
    static float isB_31=0.0f;
    static float isB_30=0.0f;
    static float isB_29=0.0f;
    static float isB_28=0.0f;
    static float isB_27=0.0f;
    static float isB_26=0.0f;
    static float isB_25=0.0f;
    static float isB_24=0.0f;
    static float isB_23=0.0f;
    static float isB_22=0.0f;
    static float isB_21=0.0f;
    static float isB_20=0.0f;
    static float isB_19=0.0f;
    static float isB_18=0.0f;
    static float isB_17=0.0f;
    static float isB_16=0.0f;
    static float isB_15=0.0f;
    static float isB_14=0.0f;
    static float isB_13=0.0f;
    static float isB_12=0.0f;
    static float isB_11=0.0f;
    static float isB_10=0.0f;
    static float isB_09=0.0f;
    static float isB_08=0.0f;
    static float isB_07=0.0f;
    static float isB_06=0.0f;
    static float isB_05=0.0f;
    static float isB_04=0.0f;
    static float isB_03=0.0f;
    static float isB_02=0.0f;
    static float isB_01=0.0f;
    static float isB_00=0.0f;
    static int   k=0;
    float filtered_isB=0.0f;

    if (k==0)
    {

    isB_49=isB;
    isB_48=isB;
    isB_47=isB;
    isB_46=isB;
    isB_45=isB;
    isB_44=isB;
    isB_43=isB;
    isB_42=isB;
    isB_41=isB;
    isB_40=isB;
    isB_39=isB;
    isB_38=isB;
    isB_37=isB;
    isB_36=isB;
    isB_35=isB;
    isB_34=isB;
    isB_33=isB;
    isB_32=isB;
    isB_31=isB;
    isB_30=isB;
    isB_29=isB;
    isB_28=isB;
    isB_27=isB;
    isB_26=isB;
    isB_25=isB;
    isB_24=isB;
    isB_23=isB;
    isB_22=isB;
    isB_21=isB;
    isB_20=isB;
    isB_19=isB;
    isB_18=isB;
    isB_17=isB;
    isB_16=isB;
    isB_15=isB;
    isB_14=isB;
    isB_13=isB;
    isB_12=isB;
    isB_11=isB;
    isB_10=isB;
    isB_09=isB;
    isB_08=isB;
    isB_07=isB;
    isB_06=isB;
    isB_05=isB;
    isB_04=isB;
    isB_03=isB;
    isB_02=isB;
    isB_01=isB;
    isB_00=isB;
    k         = 1;
    }

        isB_49=isB_48;
        isB_48=isB_47;
        isB_47=isB_46;
        isB_46=isB_45;
        isB_45=isB_44;
        isB_44=isB_43;
        isB_43=isB_42;
        isB_42=isB_41;
        isB_41=isB_40;
        isB_40=isB_39;
        isB_39=isB_38;
        isB_38=isB_37;
        isB_37=isB_36;
        isB_36=isB_35;
        isB_35=isB_34;
        isB_34=isB_33;
        isB_33=isB_32;
        isB_32=isB_31;
        isB_31=isB_30;
        isB_30=isB_29;
        isB_29=isB_28;
        isB_28=isB_27;
        isB_27=isB_26;
        isB_26=isB_25;
        isB_25=isB_24;
        isB_24=isB_23;
        isB_23=isB_22;
        isB_22=isB_21;
        isB_21=isB_20;
        isB_20=isB_19;
        isB_19=isB_18;
        isB_18=isB_17;
        isB_17=isB_16;
        isB_16=isB_15;
        isB_15=isB_14;
        isB_14=isB_13;
        isB_13=isB_12;
        isB_12=isB_11;
        isB_11=isB_10;
        isB_10=isB_09;
        isB_09=isB_08;
        isB_08=isB_07;
        isB_07=isB_06;
        isB_06=isB_05;
        isB_05=isB_04;
        isB_04=isB_03;
        isB_03=isB_02;
        isB_02=isB_01;
        isB_01=isB_00;
        isB_00=isB;

    filtered_isB = (isB_00+isB_01+isB_02+isB_03+isB_04+isB_05+isB_06+isB_07+isB_08+isB_09+
                    isB_10+isB_11+isB_12+isB_13+isB_14+isB_15+isB_16+isB_17+isB_18+isB_19+
                    isB_20+isB_21+isB_22+isB_23+isB_24+isB_25+isB_26+isB_27+isB_28+isB_29+
                    isB_30+isB_31+isB_32+isB_33+isB_34+isB_35+isB_36+isB_37+isB_38+isB_39+
                    isB_40+isB_41+isB_42+isB_43+isB_44+isB_45+isB_46+isB_47+isB_48+isB_49)/50.0f;
    
    return filtered_isB;
}


float te_moving_average_filter(float te)
{

    static float te_49=0.0f;
    static float te_48=0.0f;
    static float te_47=0.0f;
    static float te_46=0.0f;
    static float te_45=0.0f;
    static float te_44=0.0f;
    static float te_43=0.0f;
    static float te_42=0.0f;
    static float te_41=0.0f;
    static float te_40=0.0f;
    static float te_39=0.0f;
    static float te_38=0.0f;
    static float te_37=0.0f;
    static float te_36=0.0f;
    static float te_35=0.0f;
    static float te_34=0.0f;
    static float te_33=0.0f;
    static float te_32=0.0f;
    static float te_31=0.0f;
    static float te_30=0.0f;
    static float te_29=0.0f;
    static float te_28=0.0f;
    static float te_27=0.0f;
    static float te_26=0.0f;
    static float te_25=0.0f;
    static float te_24=0.0f;
    static float te_23=0.0f;
    static float te_22=0.0f;
    static float te_21=0.0f;
    static float te_20=0.0f;
    static float te_19=0.0f;
    static float te_18=0.0f;
    static float te_17=0.0f;
    static float te_16=0.0f;
    static float te_15=0.0f;
    static float te_14=0.0f;
    static float te_13=0.0f;
    static float te_12=0.0f;
    static float te_11=0.0f;
    static float te_10=0.0f;
    static float te_09=0.0f;
    static float te_08=0.0f;
    static float te_07=0.0f;
    static float te_06=0.0f;
    static float te_05=0.0f;
    static float te_04=0.0f;
    static float te_03=0.0f;
    static float te_02=0.0f;
    static float te_01=0.0f;
    static float te_00=0.0f;
    static int   k=0;
    float filtered_te=0.0f;

    if (k==0)
    {

    te_49=te;
    te_48=te;
    te_47=te;
    te_46=te;
    te_45=te;
    te_44=te;
    te_43=te;
    te_42=te;
    te_41=te;
    te_40=te;
    te_39=te;
    te_38=te;
    te_37=te;
    te_36=te;
    te_35=te;
    te_34=te;
    te_33=te;
    te_32=te;
    te_31=te;
    te_30=te;
    te_29=te;
    te_28=te;
    te_27=te;
    te_26=te;
    te_25=te;
    te_24=te;
    te_23=te;
    te_22=te;
    te_21=te;
    te_20=te;
    te_19=te;
    te_18=te;
    te_17=te;
    te_16=te;
    te_15=te;
    te_14=te;
    te_13=te;
    te_12=te;
    te_11=te;
    te_10=te;
    te_09=te;
    te_08=te;
    te_07=te;
    te_06=te;
    te_05=te;
    te_04=te;
    te_03=te;
    te_02=te;
    te_01=te;
    te_00=te;
    k         = 1;
    }

        te_49=te_48;
        te_48=te_47;
        te_47=te_46;
        te_46=te_45;
        te_45=te_44;
        te_44=te_43;
        te_43=te_42;
        te_42=te_41;
        te_41=te_40;
        te_40=te_39;
        te_39=te_38;
        te_38=te_37;
        te_37=te_36;
        te_36=te_35;
        te_35=te_34;
        te_34=te_33;
        te_33=te_32;
        te_32=te_31;
        te_31=te_30;
        te_30=te_29;
        te_29=te_28;
        te_28=te_27;
        te_27=te_26;
        te_26=te_25;
        te_25=te_24;
        te_24=te_23;
        te_23=te_22;
        te_22=te_21;
        te_21=te_20;
        te_20=te_19;
        te_19=te_18;
        te_18=te_17;
        te_17=te_16;
        te_16=te_15;
        te_15=te_14;
        te_14=te_13;
        te_13=te_12;
        te_12=te_11;
        te_11=te_10;
        te_10=te_09;
        te_09=te_08;
        te_08=te_07;
        te_07=te_06;
        te_06=te_05;
        te_05=te_04;
        te_04=te_03;
        te_03=te_02;
        te_02=te_01;
        te_01=te_00;
        te_00=te;

    filtered_te = (te_00+te_01+te_02+te_03+te_04+te_05+te_06+te_07+te_08+te_09+
                    te_10+te_11+te_12+te_13+te_14+te_15+te_16+te_17+te_18+te_19+
                    te_20+te_21+te_22+te_23+te_24+te_25+te_26+te_27+te_28+te_29+
                    te_30+te_31+te_32+te_33+te_34+te_35+te_36+te_37+te_38+te_39+
                    te_40+te_41+te_42+te_43+te_44+te_45+te_46+te_47+te_48+te_49)/50.0f;
    
    return filtered_te;
}



float wr_moving_average_filter(float wr)
{

    static float wr_49=0.0f;
    static float wr_48=0.0f;
    static float wr_47=0.0f;
    static float wr_46=0.0f;
    static float wr_45=0.0f;
    static float wr_44=0.0f;
    static float wr_43=0.0f;
    static float wr_42=0.0f;
    static float wr_41=0.0f;
    static float wr_40=0.0f;
    static float wr_39=0.0f;
    static float wr_38=0.0f;
    static float wr_37=0.0f;
    static float wr_36=0.0f;
    static float wr_35=0.0f;
    static float wr_34=0.0f;
    static float wr_33=0.0f;
    static float wr_32=0.0f;
    static float wr_31=0.0f;
    static float wr_30=0.0f;
    static float wr_29=0.0f;
    static float wr_28=0.0f;
    static float wr_27=0.0f;
    static float wr_26=0.0f;
    static float wr_25=0.0f;
    static float wr_24=0.0f;
    static float wr_23=0.0f;
    static float wr_22=0.0f;
    static float wr_21=0.0f;
    static float wr_20=0.0f;
    static float wr_19=0.0f;
    static float wr_18=0.0f;
    static float wr_17=0.0f;
    static float wr_16=0.0f;
    static float wr_15=0.0f;
    static float wr_14=0.0f;
    static float wr_13=0.0f;
    static float wr_12=0.0f;
    static float wr_11=0.0f;
    static float wr_10=0.0f;
    static float wr_09=0.0f;
    static float wr_08=0.0f;
    static float wr_07=0.0f;
    static float wr_06=0.0f;
    static float wr_05=0.0f;
    static float wr_04=0.0f;
    static float wr_03=0.0f;
    static float wr_02=0.0f;
    static float wr_01=0.0f;
    static float wr_00=0.0f;
    static int   k=0;
    float filwrred_wr=0.0f;

    if (k==0)
    {

    wr_49=wr;
    wr_48=wr;
    wr_47=wr;
    wr_46=wr;
    wr_45=wr;
    wr_44=wr;
    wr_43=wr;
    wr_42=wr;
    wr_41=wr;
    wr_40=wr;
    wr_39=wr;
    wr_38=wr;
    wr_37=wr;
    wr_36=wr;
    wr_35=wr;
    wr_34=wr;
    wr_33=wr;
    wr_32=wr;
    wr_31=wr;
    wr_30=wr;
    wr_29=wr;
    wr_28=wr;
    wr_27=wr;
    wr_26=wr;
    wr_25=wr;
    wr_24=wr;
    wr_23=wr;
    wr_22=wr;
    wr_21=wr;
    wr_20=wr;
    wr_19=wr;
    wr_18=wr;
    wr_17=wr;
    wr_16=wr;
    wr_15=wr;
    wr_14=wr;
    wr_13=wr;
    wr_12=wr;
    wr_11=wr;
    wr_10=wr;
    wr_09=wr;
    wr_08=wr;
    wr_07=wr;
    wr_06=wr;
    wr_05=wr;
    wr_04=wr;
    wr_03=wr;
    wr_02=wr;
    wr_01=wr;
    wr_00=wr;
    k         = 1;
    }

        wr_49=wr_48;
        wr_48=wr_47;
        wr_47=wr_46;
        wr_46=wr_45;
        wr_45=wr_44;
        wr_44=wr_43;
        wr_43=wr_42;
        wr_42=wr_41;
        wr_41=wr_40;
        wr_40=wr_39;
        wr_39=wr_38;
        wr_38=wr_37;
        wr_37=wr_36;
        wr_36=wr_35;
        wr_35=wr_34;
        wr_34=wr_33;
        wr_33=wr_32;
        wr_32=wr_31;
        wr_31=wr_30;
        wr_30=wr_29;
        wr_29=wr_28;
        wr_28=wr_27;
        wr_27=wr_26;
        wr_26=wr_25;
        wr_25=wr_24;
        wr_24=wr_23;
        wr_23=wr_22;
        wr_22=wr_21;
        wr_21=wr_20;
        wr_20=wr_19;
        wr_19=wr_18;
        wr_18=wr_17;
        wr_17=wr_16;
        wr_16=wr_15;
        wr_15=wr_14;
        wr_14=wr_13;
        wr_13=wr_12;
        wr_12=wr_11;
        wr_11=wr_10;
        wr_10=wr_09;
        wr_09=wr_08;
        wr_08=wr_07;
        wr_07=wr_06;
        wr_06=wr_05;
        wr_05=wr_04;
        wr_04=wr_03;
        wr_03=wr_02;
        wr_02=wr_01;
        wr_01=wr_00;
        wr_00=wr;

    filwrred_wr = (wr_00+wr_01+wr_02+wr_03+wr_04+wr_05+wr_06+wr_07+wr_08+wr_09+
                    wr_10+wr_11+wr_12+wr_13+wr_14+wr_15+wr_16+wr_17+wr_18+wr_19+
                    wr_20+wr_21+wr_22+wr_23+wr_24+wr_25+wr_26+wr_27+wr_28+wr_29+
                    wr_30+wr_31+wr_32+wr_33+wr_34+wr_35+wr_36+wr_37+wr_38+wr_39+
                    wr_40+wr_41+wr_42+wr_43+wr_44+wr_45+wr_46+wr_47+wr_48+wr_49)/50.0f;
    
    return filwrred_wr;
}







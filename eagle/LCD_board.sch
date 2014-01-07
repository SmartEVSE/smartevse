<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="con-lstb">
<description>&lt;b&gt;Pin Headers&lt;/b&gt;&lt;p&gt;
Naming:&lt;p&gt;
MA = male&lt;p&gt;
# contacts - # rows&lt;p&gt;
W = angled&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="MA08-2">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-9.525" y1="2.54" x2="-8.255" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-8.255" y1="2.54" x2="-7.62" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="1.905" x2="-6.985" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="2.54" x2="-5.715" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="2.54" x2="-5.08" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-9.525" y1="2.54" x2="-10.16" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="1.905" x2="-4.445" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="2.54" x2="-3.175" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="2.54" x2="-2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.54" x2="-0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="2.54" x2="0" y2="1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="1.905" x2="0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0.635" y1="2.54" x2="1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="2.54" x2="2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.54" x2="-2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.905" x2="3.175" y2="2.54" width="0.1524" layer="21"/>
<wire x1="3.175" y1="2.54" x2="4.445" y2="2.54" width="0.1524" layer="21"/>
<wire x1="4.445" y1="2.54" x2="5.08" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="-1.905" x2="-8.255" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-1.905" x2="-5.715" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-2.54" x2="-6.985" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="-2.54" x2="-7.62" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="1.905" x2="-10.16" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-10.16" y1="-1.905" x2="-9.525" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-8.255" y1="-2.54" x2="-9.525" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-3.175" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-2.54" x2="-4.445" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-2.54" x2="-5.08" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="-0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-2.54" x2="0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-2.54" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-2.54" x2="-1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.905" x2="4.445" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-2.54" x2="3.175" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-2.54" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="5.715" y1="2.54" x2="6.985" y2="2.54" width="0.1524" layer="21"/>
<wire x1="6.985" y1="2.54" x2="7.62" y2="1.905" width="0.1524" layer="21"/>
<wire x1="7.62" y1="1.905" x2="8.255" y2="2.54" width="0.1524" layer="21"/>
<wire x1="8.255" y1="2.54" x2="9.525" y2="2.54" width="0.1524" layer="21"/>
<wire x1="9.525" y1="2.54" x2="10.16" y2="1.905" width="0.1524" layer="21"/>
<wire x1="5.715" y1="2.54" x2="5.08" y2="1.905" width="0.1524" layer="21"/>
<wire x1="10.16" y1="1.905" x2="10.16" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-1.905" x2="6.985" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="10.16" y1="-1.905" x2="9.525" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="9.525" y1="-2.54" x2="8.255" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="8.255" y1="-2.54" x2="7.62" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.905" x2="5.715" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-2.54" x2="5.715" y2="-2.54" width="0.1524" layer="21"/>
<pad name="1" x="-8.89" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="-6.35" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="5" x="-3.81" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="7" x="-1.27" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="9" x="1.27" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="11" x="3.81" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-8.89" y="1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="-6.35" y="1.27" drill="1.016" shape="octagon"/>
<pad name="6" x="-3.81" y="1.27" drill="1.016" shape="octagon"/>
<pad name="8" x="-1.27" y="1.27" drill="1.016" shape="octagon"/>
<pad name="10" x="1.27" y="1.27" drill="1.016" shape="octagon"/>
<pad name="12" x="3.81" y="1.27" drill="1.016" shape="octagon"/>
<pad name="13" x="6.35" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="15" x="8.89" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="14" x="6.35" y="1.27" drill="1.016" shape="octagon"/>
<pad name="16" x="8.89" y="1.27" drill="1.016" shape="octagon"/>
<text x="-9.398" y="-4.191" size="1.27" layer="21" ratio="10">1</text>
<text x="-10.16" y="2.921" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="7.62" y="2.921" size="1.27" layer="21" ratio="10">16</text>
<text x="0" y="-4.191" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-6.604" y1="-1.524" x2="-6.096" y2="-1.016" layer="51"/>
<rectangle x1="-9.144" y1="-1.524" x2="-8.636" y2="-1.016" layer="51"/>
<rectangle x1="-4.064" y1="-1.524" x2="-3.556" y2="-1.016" layer="51"/>
<rectangle x1="1.016" y1="-1.524" x2="1.524" y2="-1.016" layer="51"/>
<rectangle x1="-1.524" y1="-1.524" x2="-1.016" y2="-1.016" layer="51"/>
<rectangle x1="3.556" y1="-1.524" x2="4.064" y2="-1.016" layer="51"/>
<rectangle x1="-9.144" y1="1.016" x2="-8.636" y2="1.524" layer="51"/>
<rectangle x1="-6.604" y1="1.016" x2="-6.096" y2="1.524" layer="51"/>
<rectangle x1="-4.064" y1="1.016" x2="-3.556" y2="1.524" layer="51"/>
<rectangle x1="-1.524" y1="1.016" x2="-1.016" y2="1.524" layer="51"/>
<rectangle x1="1.016" y1="1.016" x2="1.524" y2="1.524" layer="51"/>
<rectangle x1="3.556" y1="1.016" x2="4.064" y2="1.524" layer="51"/>
<rectangle x1="8.636" y1="-1.524" x2="9.144" y2="-1.016" layer="51"/>
<rectangle x1="6.096" y1="-1.524" x2="6.604" y2="-1.016" layer="51"/>
<rectangle x1="6.096" y1="1.016" x2="6.604" y2="1.524" layer="51"/>
<rectangle x1="8.636" y1="1.016" x2="9.144" y2="1.524" layer="51"/>
</package>
<package name="MA06-1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-6.985" y1="1.27" x2="-5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0.635" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="0.635" x2="-7.62" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="1.27" x2="-7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="-0.635" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-1.27" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-0.635" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0.635" x2="5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.985" y2="1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="1.27" x2="7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-0.635" x2="6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-1.27" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="0.635" x2="7.62" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="2" x="-3.81" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="3" x="-1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="4" x="1.27" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="5" x="3.81" y="0" drill="1.016" shape="long" rot="R90"/>
<pad name="6" x="6.35" y="0" drill="1.016" shape="long" rot="R90"/>
<text x="-7.62" y="1.651" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.985" y="-2.921" size="1.27" layer="21" ratio="10">1</text>
<text x="5.715" y="1.651" size="1.27" layer="21" ratio="10">6</text>
<text x="-2.54" y="-2.921" size="1.27" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-4.064" y1="-0.254" x2="-3.556" y2="0.254" layer="51"/>
<rectangle x1="-6.604" y1="-0.254" x2="-6.096" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="3.556" y1="-0.254" x2="4.064" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="6.096" y1="-0.254" x2="6.604" y2="0.254" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="MA08-2">
<wire x1="3.81" y1="-10.16" x2="-3.81" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-2.54" x2="-1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-5.08" x2="-1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="-7.62" x2="-1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="2.54" x2="-1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="10.16" x2="2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="1.27" y1="7.62" x2="2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="10.16" x2="-1.27" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="7.62" x2="-1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-2.54" y1="5.08" x2="-1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-3.81" y1="12.7" x2="-3.81" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-10.16" x2="3.81" y2="12.7" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="12.7" x2="3.81" y2="12.7" width="0.4064" layer="94"/>
<text x="-3.81" y="-12.7" size="1.778" layer="96">&gt;VALUE</text>
<text x="-3.81" y="13.462" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="3" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="5" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="4" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="6" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="7" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="9" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="8" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="10" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="11" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="13" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="15" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="12" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="14" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="16" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
</symbol>
<symbol name="MA06-1">
<wire x1="3.81" y1="-10.16" x2="-1.27" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="1.27" y1="2.54" x2="2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="1.27" y1="0" x2="2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="1.27" y1="5.08" x2="2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-1.27" y2="-10.16" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-10.16" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="3.81" y2="7.62" width="0.4064" layer="94"/>
<text x="-1.27" y="-12.7" size="1.778" layer="96">&gt;VALUE</text>
<text x="-1.27" y="8.382" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="2" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="3" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="5" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="MA08-2" prefix="SV" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="MA08-2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MA08-2">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="15" pad="15"/>
<connect gate="G$1" pin="16" pad="16"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="MA06-1" prefix="SV" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="MA06-1" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MA06-1">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
<connect gate="1" pin="3" pad="3"/>
<connect gate="1" pin="4" pad="4"/>
<connect gate="1" pin="5" pad="5"/>
<connect gate="1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name="">
<attribute name="MF" value="" constant="no"/>
<attribute name="MPN" value="" constant="no"/>
<attribute name="OC_FARNELL" value="unknown" constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="VCC">
<wire x1="1.27" y1="-1.905" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.27" y2="-1.905" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96" rot="R90">&gt;VALUE</text>
<pin name="VCC" x="0" y="-2.54" visible="off" length="short" direction="sup" rot="R90"/>
</symbol>
<symbol name="GND">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="VCC" prefix="P+">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="VCC" symbol="VCC" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GND" prefix="GND">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Panasonic-Trimmer_Potentiometers">
<description>&lt;b&gt; Developed By element14 &lt;/b&gt;&lt;p&gt; 

For feedback and Technical support please contact reach from below:
&lt;br&gt; Tech Support e-mail  : &lt;a href="mailto:CAD_Tech@element14.com"&gt;CAD_Tech@element14.com&lt;/a&gt;
&lt;br&gt;Community website: &lt;a href=http://www.element14.com/community/community/knode/cad_tools/cadsoft_eagle/eagle_cad_libraries&gt;
EAGLE CAD Libraries on element14&lt;/a&gt;</description>
<packages>
<package name="POT_EVM1D">
<smd name="1" x="-1.4478" y="-1.8542" dx="1.2954" dy="1.4986" layer="1"/>
<smd name="2" x="0" y="1.9558" dx="2.3876" dy="1.1938" layer="1"/>
<smd name="3" x="1.4478" y="-1.8542" dx="1.2954" dy="1.4986" layer="1"/>
<wire x1="2.3622" y1="-2.8448" x2="-2.3622" y2="-2.8448" width="0.1524" layer="39"/>
<wire x1="-2.3622" y1="-2.8448" x2="-2.3622" y2="2.794" width="0.1524" layer="39"/>
<wire x1="-2.3622" y1="2.794" x2="2.3622" y2="2.794" width="0.1524" layer="39"/>
<wire x1="2.3622" y1="2.794" x2="2.3622" y2="-2.8448" width="0.1524" layer="39"/>
<wire x1="-1.524" y1="2.2606" x2="-1.905" y2="2.2606" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.2606" x2="-1.905" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="1.905" y1="2.2606" x2="1.905" y2="-0.762" width="0.1524" layer="21"/>
<wire x1="1.905" y1="2.2606" x2="1.524" y2="2.2606" width="0.1524" layer="21"/>
<wire x1="-0.4572" y1="-2.2606" x2="0.4572" y2="-2.2606" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.2606" x2="-1.905" y2="-2.2606" width="0.1524" layer="51"/>
<wire x1="1.905" y1="2.2606" x2="1.905" y2="-2.2606" width="0.1524" layer="51"/>
<wire x1="1.905" y1="2.2606" x2="-1.905" y2="2.2606" width="0.1524" layer="51"/>
<wire x1="-1.905" y1="-2.2606" x2="1.905" y2="-2.2606" width="0.1524" layer="51"/>
<wire x1="-0.2286" y1="1.143" x2="-0.2286" y2="0.2286" width="0.1524" layer="51"/>
<wire x1="-0.2286" y1="1.143" x2="0.2286" y2="1.143" width="0.1524" layer="51"/>
<wire x1="0.2286" y1="1.143" x2="0.2286" y2="0.2286" width="0.1524" layer="51"/>
<wire x1="0.2286" y1="0.2286" x2="1.143" y2="0.2286" width="0.1524" layer="51"/>
<wire x1="1.143" y1="0.2286" x2="1.143" y2="-0.2286" width="0.1524" layer="51"/>
<wire x1="0.2286" y1="-0.2286" x2="1.143" y2="-0.2286" width="0.1524" layer="51"/>
<wire x1="0.2286" y1="-1.143" x2="0.2286" y2="-0.2286" width="0.1524" layer="51"/>
<wire x1="-1.143" y1="0.2286" x2="-1.143" y2="-0.2286" width="0.1524" layer="51"/>
<wire x1="-0.2286" y1="-1.143" x2="-0.2286" y2="-0.2286" width="0.1524" layer="51"/>
<wire x1="-0.2286" y1="0.2286" x2="-1.143" y2="0.2286" width="0.1524" layer="51"/>
<wire x1="-0.2286" y1="-1.143" x2="0.2286" y2="-1.143" width="0.1524" layer="51"/>
<wire x1="-0.2286" y1="-0.2286" x2="-1.143" y2="-0.2286" width="0.1524" layer="51"/>
<wire x1="1.4224" y1="0" x2="-1.4224" y2="0" width="0.1524" layer="51" curve="-180"/>
<wire x1="-1.4224" y1="0" x2="1.4224" y2="0" width="0.1524" layer="51" curve="-180"/>
<text x="-2.9972" y="3.1242" size="1.27" layer="25" ratio="6" rot="SR0">&gt;NAME</text>
<text x="-3.6068" y="-4.6736" size="1.27" layer="27" ratio="6" rot="SR0">&gt;VALUE</text>
</package>
<package name="POT_EVM2N">
<smd name="1" x="-0.6604" y="-1.4478" dx="0.6096" dy="0.889" layer="1"/>
<smd name="2" x="0" y="1.0922" dx="1.1938" dy="0.889" layer="1"/>
<smd name="3" x="0.6604" y="-1.4478" dx="0.6096" dy="0.889" layer="1"/>
<wire x1="1.2192" y1="-2.159" x2="-1.2192" y2="-2.159" width="0.1524" layer="39"/>
<wire x1="-1.2192" y1="-2.159" x2="-1.2192" y2="1.8034" width="0.1524" layer="39"/>
<wire x1="-1.2192" y1="1.8034" x2="1.2192" y2="1.8034" width="0.1524" layer="39"/>
<wire x1="1.2192" y1="1.8034" x2="1.2192" y2="-2.159" width="0.1524" layer="39"/>
<wire x1="-1.0922" y1="1.397" x2="-1.0922" y2="-0.6858" width="0.1524" layer="21"/>
<wire x1="1.0922" y1="1.397" x2="1.0922" y2="-0.6858" width="0.1524" layer="21"/>
<wire x1="1.0922" y1="1.397" x2="0.9398" y2="1.397" width="0.1524" layer="21"/>
<wire x1="-0.9398" y1="1.397" x2="-1.0922" y2="1.397" width="0.1524" layer="21"/>
<wire x1="-1.0922" y1="1.397" x2="-1.0922" y2="-1.397" width="0.1524" layer="51"/>
<wire x1="1.0922" y1="1.397" x2="1.0922" y2="-1.397" width="0.1524" layer="51"/>
<wire x1="1.0922" y1="1.397" x2="-1.0922" y2="1.397" width="0.1524" layer="51"/>
<wire x1="-1.0922" y1="-1.397" x2="1.0922" y2="-1.397" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="0.6604" x2="-0.127" y2="0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="0.6604" x2="0.127" y2="0.6604" width="0.1524" layer="51"/>
<wire x1="0.127" y1="0.6604" x2="0.127" y2="0.127" width="0.1524" layer="51"/>
<wire x1="0.127" y1="0.127" x2="0.6604" y2="0.127" width="0.1524" layer="51"/>
<wire x1="0.6604" y1="0.127" x2="0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="0.127" y1="-0.127" x2="0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="0.127" y1="-0.6604" x2="0.127" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="-0.6604" y1="0.127" x2="-0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="-0.6604" x2="-0.127" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="0.127" x2="-0.6604" y2="0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="-0.6604" x2="0.127" y2="-0.6604" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="-0.127" x2="-0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="0.8128" y1="0" x2="-0.8128" y2="0" width="0.1524" layer="51" curve="-180"/>
<wire x1="-0.8128" y1="0" x2="0.8128" y2="0" width="0.1524" layer="51" curve="-180"/>
<text x="-2.9464" y="2.0828" size="1.27" layer="25" ratio="6" rot="SR0">&gt;NAME</text>
<text x="-3.4036" y="-4.0132" size="1.27" layer="27" ratio="6" rot="SR0">&gt;VALUE</text>
</package>
<package name="POT_EVM2W">
<smd name="1" x="-0.6604" y="-1.2954" dx="0.6096" dy="0.889" layer="1"/>
<smd name="2" x="0" y="1.1938" dx="1.1938" dy="1.0922" layer="1"/>
<smd name="3" x="0.6604" y="-1.2954" dx="0.6096" dy="0.889" layer="1"/>
<wire x1="1.2192" y1="-2.0066" x2="-1.2192" y2="-2.0066" width="0.1524" layer="39"/>
<wire x1="-1.2192" y1="-2.0066" x2="-1.2192" y2="2.0066" width="0.1524" layer="39"/>
<wire x1="-1.2192" y1="2.0066" x2="1.2192" y2="2.0066" width="0.1524" layer="39"/>
<wire x1="1.2192" y1="2.0066" x2="1.2192" y2="-2.0066" width="0.1524" layer="39"/>
<wire x1="-1.0922" y1="1.3716" x2="-1.0922" y2="-0.5588" width="0.1524" layer="21"/>
<wire x1="1.0922" y1="1.3716" x2="1.0922" y2="-0.5588" width="0.1524" layer="21"/>
<wire x1="1.0922" y1="1.3716" x2="0.9398" y2="1.3716" width="0.1524" layer="21"/>
<wire x1="-0.9398" y1="1.3716" x2="-1.0922" y2="1.3716" width="0.1524" layer="21"/>
<wire x1="-1.0922" y1="1.3716" x2="-1.0922" y2="-1.3716" width="0.1524" layer="51"/>
<wire x1="1.0922" y1="1.3716" x2="1.0922" y2="-1.3716" width="0.1524" layer="51"/>
<wire x1="1.0922" y1="1.3716" x2="-1.0922" y2="1.3716" width="0.1524" layer="51"/>
<wire x1="-1.0922" y1="-1.3716" x2="1.0922" y2="-1.3716" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="0.6604" x2="-0.127" y2="0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="0.6604" x2="0.127" y2="0.6604" width="0.1524" layer="51"/>
<wire x1="0.127" y1="0.6604" x2="0.127" y2="0.127" width="0.1524" layer="51"/>
<wire x1="0.127" y1="0.127" x2="0.6604" y2="0.127" width="0.1524" layer="51"/>
<wire x1="0.6604" y1="0.127" x2="0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="0.127" y1="-0.127" x2="0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="0.127" y1="-0.6604" x2="0.127" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="-0.6604" y1="0.127" x2="-0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="-0.6604" x2="-0.127" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="0.127" x2="-0.6604" y2="0.127" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="-0.6604" x2="0.127" y2="-0.6604" width="0.1524" layer="51"/>
<wire x1="-0.127" y1="-0.127" x2="-0.6604" y2="-0.127" width="0.1524" layer="51"/>
<wire x1="0.8128" y1="0" x2="-0.8128" y2="0" width="0.1524" layer="51" curve="-180"/>
<wire x1="-0.8128" y1="0" x2="0.8128" y2="0" width="0.1524" layer="51" curve="-180"/>
<text x="-2.8448" y="2.5908" size="1.27" layer="25" ratio="6" rot="SR0">&gt;NAME</text>
<text x="-3.6068" y="-3.6576" size="1.27" layer="27" ratio="6" rot="SR0">&gt;VALUE</text>
</package>
<package name="POT_EVM3W">
<smd name="1" x="-0.9906" y="-1.6002" dx="1.1938" dy="1.1938" layer="1"/>
<smd name="2" x="0" y="1.651" dx="1.6002" dy="1.4986" layer="1"/>
<smd name="3" x="0.9906" y="-1.6002" dx="1.1938" dy="1.1938" layer="1"/>
<wire x1="1.1684" y1="0" x2="-1.1684" y2="0" width="0.1524" layer="51" curve="-180"/>
<wire x1="-1.1684" y1="0" x2="1.1684" y2="0" width="0.1524" layer="51" curve="-180"/>
<text x="-2.8956" y="3.1496" size="1.27" layer="25" ratio="6" rot="SR0">&gt;NAME</text>
<text x="-3.556" y="-4.0894" size="1.27" layer="27" ratio="6" rot="SR0">&gt;VALUE</text>
<wire x1="1.8542" y1="-2.4638" x2="-1.8542" y2="-2.4638" width="0.1524" layer="39"/>
<wire x1="-1.8542" y1="-2.4638" x2="-1.8542" y2="2.6416" width="0.1524" layer="39"/>
<wire x1="-1.8542" y1="2.6416" x2="1.8542" y2="2.6416" width="0.1524" layer="39"/>
<wire x1="1.8542" y1="2.6416" x2="1.8542" y2="-2.4638" width="0.1524" layer="39"/>
<wire x1="-1.5494" y1="1.8796" x2="-1.5494" y2="-0.6604" width="0.1524" layer="21"/>
<wire x1="1.5494" y1="1.8796" x2="1.5494" y2="-0.6604" width="0.1524" layer="21"/>
<wire x1="1.5494" y1="1.8796" x2="1.143" y2="1.8796" width="0.1524" layer="21"/>
<wire x1="-1.143" y1="1.8796" x2="-1.5494" y2="1.8796" width="0.1524" layer="21"/>
<wire x1="-1.5494" y1="1.8796" x2="-1.5494" y2="-1.8796" width="0.1524" layer="51"/>
<wire x1="1.5494" y1="1.8796" x2="1.5494" y2="-1.8796" width="0.1524" layer="51"/>
<wire x1="1.5494" y1="1.8796" x2="-1.5494" y2="1.8796" width="0.1524" layer="51"/>
<wire x1="-1.5494" y1="-1.8796" x2="1.5494" y2="-1.8796" width="0.1524" layer="51"/>
<wire x1="-0.2032" y1="0.9398" x2="-0.2032" y2="0.2032" width="0.1524" layer="51"/>
<wire x1="-0.2032" y1="0.9398" x2="0.2032" y2="0.9398" width="0.1524" layer="51"/>
<wire x1="0.2032" y1="0.9398" x2="0.2032" y2="0.2032" width="0.1524" layer="51"/>
<wire x1="0.2032" y1="0.2032" x2="0.9398" y2="0.2032" width="0.1524" layer="51"/>
<wire x1="0.9398" y1="0.2032" x2="0.9398" y2="-0.2032" width="0.1524" layer="51"/>
<wire x1="0.2032" y1="-0.2032" x2="0.9398" y2="-0.2032" width="0.1524" layer="51"/>
<wire x1="0.2032" y1="-0.9398" x2="0.2032" y2="-0.2032" width="0.1524" layer="51"/>
<wire x1="-0.9398" y1="0.2032" x2="-0.9398" y2="-0.2032" width="0.1524" layer="51"/>
<wire x1="-0.2032" y1="-0.9398" x2="-0.2032" y2="-0.2032" width="0.1524" layer="51"/>
<wire x1="-0.2032" y1="0.2032" x2="-0.9398" y2="0.2032" width="0.1524" layer="51"/>
<wire x1="-0.2032" y1="-0.9398" x2="0.2032" y2="-0.9398" width="0.1524" layer="51"/>
<wire x1="-0.2032" y1="-0.2032" x2="-0.9398" y2="-0.2032" width="0.1524" layer="51"/>
<wire x1="1.1684" y1="0" x2="-1.1684" y2="0" width="0.1524" layer="51" curve="-180"/>
<wire x1="-1.1684" y1="0" x2="1.1684" y2="0" width="0.1524" layer="51" curve="-180"/>
<text x="-2.8956" y="3.1496" size="1.27" layer="25" ratio="6" rot="SR0">&gt;NAME</text>
<text x="-3.556" y="-4.0894" size="1.27" layer="27" ratio="6" rot="SR0">&gt;VALUE</text>
</package>
<package name="POT_EVMAAS">
<pad name="1" x="-5.0038" y="0" drill="1.3208" shape="square"/>
<pad name="2" x="-2.4892" y="2.9972" drill="1.3208"/>
<pad name="3" x="0" y="0" drill="1.3208"/>
<smd name="4" x="-2.4892" y="0.2794" dx="0" dy="0" layer="1" roundness="100"/>
<wire x1="1.4986" y1="-4.4196" x2="-6.5024" y2="-4.4196" width="0.1524" layer="39"/>
<wire x1="-6.5024" y1="-4.4196" x2="-6.5024" y2="4.7498" width="0.1524" layer="39"/>
<wire x1="-6.5024" y1="4.7498" x2="1.4986" y2="4.7498" width="0.1524" layer="39"/>
<wire x1="1.4986" y1="4.7498" x2="1.4986" y2="-4.4196" width="0.1524" layer="39"/>
<wire x1="0.9398" y1="1.27" x2="0.9398" y2="3.9116" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="3.9116" x2="-5.9436" y2="3.9116" width="0.1524" layer="21"/>
<wire x1="-5.9436" y1="-1.5748" x2="-5.9436" y2="-3.9116" width="0.1524" layer="21"/>
<wire x1="-5.9436" y1="-3.9116" x2="0.9398" y2="-3.9116" width="0.1524" layer="21"/>
<wire x1="0.9398" y1="-3.9116" x2="0.9398" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.9398" y1="3.9116" x2="-1.1938" y2="3.9116" width="0.1524" layer="21"/>
<wire x1="-5.9436" y1="3.9116" x2="-5.9436" y2="1.5748" width="0.1524" layer="21"/>
<wire x1="-2.921" y1="2.3622" x2="-2.921" y2="0.7112" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="2.3622" x2="-2.0574" y2="2.3622" width="0.1524" layer="51"/>
<wire x1="-2.0574" y1="2.3622" x2="-2.0574" y2="0.7112" width="0.1524" layer="51"/>
<wire x1="-2.0574" y1="0.7112" x2="-0.4318" y2="0.7112" width="0.1524" layer="51"/>
<wire x1="-0.4318" y1="0.7112" x2="-0.4318" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-2.0574" y1="-0.1524" x2="-0.4318" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-2.0574" y1="-1.778" x2="-2.0574" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-4.572" y1="0.7112" x2="-4.572" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="-1.778" x2="-2.921" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="0.7112" x2="-4.572" y2="0.7112" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="-1.778" x2="-2.0574" y2="-1.778" width="0.1524" layer="51"/>
<wire x1="-2.921" y1="-0.1524" x2="-4.572" y2="-0.1524" width="0.1524" layer="51"/>
<wire x1="-5.9436" y1="-3.9116" x2="0.9398" y2="-3.9116" width="0.1524" layer="51"/>
<wire x1="0.9398" y1="-3.9116" x2="0.9398" y2="3.9116" width="0.1524" layer="51"/>
<wire x1="0.9398" y1="3.9116" x2="-5.9436" y2="3.9116" width="0.1524" layer="51"/>
<wire x1="-5.9436" y1="3.9116" x2="-5.9436" y2="-3.9116" width="0.1524" layer="51"/>
<wire x1="0.0762" y1="0.2794" x2="-5.08" y2="0.2794" width="0.1524" layer="51" curve="-180"/>
<wire x1="-5.08" y1="0.2794" x2="0.0762" y2="0.2794" width="0.1524" layer="51" curve="-180"/>
<text x="-5.4864" y="5.3086" size="1.27" layer="25" ratio="6" rot="SR0">&gt;NAME</text>
<text x="-6.0198" y="-6.0452" size="1.27" layer="27" ratio="6" rot="SR0">&gt;VALUE</text>
</package>
</packages>
<symbols>
<symbol name="POT">
<pin name="1" x="0" y="0" visible="off" length="short" direction="pas"/>
<pin name="2" x="10.16" y="7.62" visible="off" length="short" direction="pas" rot="R270"/>
<pin name="3" x="20.32" y="0" visible="off" length="short" direction="pas" rot="R180"/>
<wire x1="9.525" y1="3.81" x2="10.16" y2="1.905" width="0.2032" layer="94"/>
<wire x1="10.795" y1="3.81" x2="10.16" y2="1.905" width="0.2032" layer="94"/>
<wire x1="9.525" y1="3.81" x2="10.16" y2="3.81" width="0.2032" layer="94"/>
<wire x1="10.16" y1="3.81" x2="10.795" y2="3.81" width="0.2032" layer="94"/>
<wire x1="10.16" y1="3.81" x2="10.16" y2="5.08" width="0.2032" layer="94"/>
<wire x1="6.35" y1="0" x2="6.985" y2="1.27" width="0.2032" layer="94"/>
<wire x1="9.525" y1="1.27" x2="10.795" y2="-1.27" width="0.2032" layer="94"/>
<wire x1="10.795" y1="-1.27" x2="12.065" y2="1.27" width="0.2032" layer="94"/>
<wire x1="8.255" y1="-1.27" x2="9.525" y2="1.27" width="0.2032" layer="94"/>
<wire x1="6.985" y1="1.27" x2="8.255" y2="-1.27" width="0.2032" layer="94"/>
<wire x1="13.335" y1="-1.27" x2="13.97" y2="0" width="0.2032" layer="94"/>
<wire x1="13.97" y1="0" x2="17.78" y2="0" width="0.2032" layer="94"/>
<wire x1="2.54" y1="0" x2="6.35" y2="0" width="0.2032" layer="94"/>
<wire x1="12.065" y1="1.27" x2="13.335" y2="-1.27" width="0.2032" layer="94"/>
<text x="7.62" y="12.7" size="2.032" layer="95">&gt;Name</text>
<text x="7.62" y="-5.08" size="2.032" layer="96">&gt;Value</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="EVM" prefix="R">
<description>Potentiometers</description>
<gates>
<gate name="A" symbol="POT" x="0" y="0"/>
</gates>
<devices>
<device name="-1D" package="POT_EVM1D">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<technologies>
<technology name="1DSX30B12">
<attribute name="MPN" value="EVM1DSX30B12" constant="no"/>
<attribute name="OC_FARNELL" value="1849809  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0097" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100OHM " constant="no"/>
</technology>
<technology name="1DSX30B13">
<attribute name="MPN" value="EVM1DSX30B13" constant="no"/>
<attribute name="OC_FARNELL" value="1849812  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0098" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1KOHM " constant="no"/>
</technology>
<technology name="1DSX30B14">
<attribute name="MPN" value="EVM1DSX30B14" constant="no"/>
<attribute name="OC_FARNELL" value="1849816  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0099" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 10KOHM " constant="no"/>
</technology>
<technology name="1DSX30B15">
<attribute name="MPN" value="EVM1DSX30B15" constant="no"/>
<attribute name="OC_FARNELL" value="1849819 " constant="no"/>
<attribute name="OC_NEWARK" value="48W0100 " constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100KOHM " constant="no"/>
</technology>
<technology name="1DSX30B16">
<attribute name="MPN" value="EVM1DSX30B16" constant="no"/>
<attribute name="OC_FARNELL" value="1849822  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0101" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1MOHM " constant="no"/>
</technology>
<technology name="1DSX30B22">
<attribute name="MPN" value="EVM1DSX30B22" constant="no"/>
<attribute name="OC_FARNELL" value="1849810  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0102" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200OHM " constant="no"/>
</technology>
<technology name="1DSX30B23">
<attribute name="MPN" value="EVM1DSX30B23" constant="no"/>
<attribute name="OC_FARNELL" value="1849813  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0103" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 2KOHM " constant="no"/>
</technology>
<technology name="1DSX30B24">
<attribute name="MPN" value="EVM1DSX30B24" constant="no"/>
<attribute name="OC_FARNELL" value="1849817  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0104" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 20KOHM " constant="no"/>
</technology>
<technology name="1DSX30B25">
<attribute name="MPN" value="EVM1DSX30B25" constant="no"/>
<attribute name="OC_FARNELL" value="1849820  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0105" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200KOHM " constant="no"/>
</technology>
<technology name="1DSX30B52">
<attribute name="MPN" value="EVM1DSX30B52" constant="no"/>
<attribute name="OC_FARNELL" value="1849811  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0106" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500OHM " constant="no"/>
</technology>
<technology name="1DSX30B53">
<attribute name="MPN" value="EVM1DSX30B53" constant="no"/>
<attribute name="OC_FARNELL" value="1849814  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0107" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 5KOHM " constant="no"/>
</technology>
<technology name="1DSX30B54">
<attribute name="MPN" value="EVM1DSX30B54" constant="no"/>
<attribute name="OC_FARNELL" value="1849818  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0108" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 50KOHM " constant="no"/>
</technology>
<technology name="1DSX30B55">
<attribute name="MPN" value="EVM1DSX30B55" constant="no"/>
<attribute name="OC_FARNELL" value="1849821  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0109" constant="no"/>
<attribute name="PACKAGE" value="EVM1D" constant="no"/>
<attribute name="POWERRATING" value=" 0.2W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500KOHM " constant="no"/>
</technology>
</technologies>
</device>
<device name="-2N" package="POT_EVM2N">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<technologies>
<technology name="2NSX80B12">
<attribute name="MPN" value="EVM2NSX80B12" constant="no"/>
<attribute name="OC_FARNELL" value="1849752  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5154" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100OHM " constant="no"/>
</technology>
<technology name="2NSX80B13">
<attribute name="MPN" value="EVM2NSX80B13" constant="no"/>
<attribute name="OC_FARNELL" value="1849755  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5155" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1KOHM " constant="no"/>
</technology>
<technology name="2NSX80B15">
<attribute name="MPN" value="EVM2NSX80B15" constant="no"/>
<attribute name="OC_FARNELL" value="1849762  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5157" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100KOHM " constant="no"/>
</technology>
<technology name="2NSX80B16">
<attribute name="MPN" value="EVM2NSX80B16" constant="no"/>
<attribute name="OC_FARNELL" value="1849765  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5158" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1MOHM " constant="no"/>
</technology>
<technology name="2NSX80B22">
<attribute name="MPN" value="EVM2NSX80B22" constant="no"/>
<attribute name="OC_FARNELL" value="1849753  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5159" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200OHM " constant="no"/>
</technology>
<technology name="2NSX80B23">
<attribute name="MPN" value="EVM2NSX80B23" constant="no"/>
<attribute name="OC_FARNELL" value="1849756  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5160" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 2KOHM " constant="no"/>
</technology>
<technology name="2NSX80B24">
<attribute name="MPN" value="EVM2NSX80B24" constant="no"/>
<attribute name="OC_FARNELL" value="1849759  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5161" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 20KOHM " constant="no"/>
</technology>
<technology name="2NSX80B25">
<attribute name="MPN" value="EVM2NSX80B25" constant="no"/>
<attribute name="OC_FARNELL" value="1849763  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5162" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200KOHM " constant="no"/>
</technology>
<technology name="2NSX80B52">
<attribute name="MPN" value="EVM2NSX80B52" constant="no"/>
<attribute name="OC_FARNELL" value="1849754  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5163" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500OHM " constant="no"/>
</technology>
<technology name="2NSX80B53">
<attribute name="MPN" value="EVM2NSX80B53" constant="no"/>
<attribute name="OC_FARNELL" value="1849757  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5164" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 5KOHM " constant="no"/>
</technology>
<technology name="2NSX80B54">
<attribute name="MPN" value="EVM2NSX80B54" constant="no"/>
<attribute name="OC_FARNELL" value="1849760  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5165" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 50KOHM " constant="no"/>
</technology>
<technology name="2NSX80B55">
<attribute name="MPN" value="EVM2NSX80B55" constant="no"/>
<attribute name="OC_FARNELL" value="1849764  " constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
<attribute name="PACKAGE" value="EVM2N" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500KOHM " constant="no"/>
</technology>
</technologies>
</device>
<device name="-2W" package="POT_EVM2W">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<technologies>
<technology name="2WSX80B12">
<attribute name="MPN" value="EVM2WSX80B12" constant="no"/>
<attribute name="OC_FARNELL" value="1849766  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5167" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100OHM " constant="no"/>
</technology>
<technology name="2WSX80B14">
<attribute name="MPN" value="EVM2WSX80B14" constant="no"/>
<attribute name="OC_FARNELL" value="1849772  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5169" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 10KOHM " constant="no"/>
</technology>
<technology name="2WSX80B15">
<attribute name="MPN" value="EVM2WSX80B15" constant="no"/>
<attribute name="OC_FARNELL" value="1849776  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5170" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100KOHM " constant="no"/>
</technology>
<technology name="2WSX80B16">
<attribute name="MPN" value="EVM2WSX80B16" constant="no"/>
<attribute name="OC_FARNELL" value="1849779  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5171" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1MOHM " constant="no"/>
</technology>
<technology name="2WSX80B22">
<attribute name="MPN" value="EVM2WSX80B22" constant="no"/>
<attribute name="OC_FARNELL" value="1849767  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5172" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200OHM " constant="no"/>
</technology>
<technology name="2WSX80B23">
<attribute name="MPN" value="EVM2WSX80B23" constant="no"/>
<attribute name="OC_FARNELL" value="1849770  " constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 2KOHM " constant="no"/>
</technology>
<technology name="2WSX80B24">
<attribute name="MPN" value="EVM2WSX80B24" constant="no"/>
<attribute name="OC_FARNELL" value="1849774  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5174" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 20KOHM " constant="no"/>
</technology>
<technology name="2WSX80B25">
<attribute name="MPN" value="EVM2WSX80B25" constant="no"/>
<attribute name="OC_FARNELL" value="1849777  " constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200KOHM " constant="no"/>
</technology>
<technology name="2WSX80B52">
<attribute name="MPN" value="EVM2WSX80B52" constant="no"/>
<attribute name="OC_FARNELL" value="1849768  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5176" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500OHM " constant="no"/>
</technology>
<technology name="2WSX80B53">
<attribute name="MPN" value="EVM2WSX80B53" constant="no"/>
<attribute name="OC_FARNELL" value="1849771  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5177" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 5KOHM " constant="no"/>
</technology>
<technology name="2WSX80B54">
<attribute name="MPN" value="EVM2WSX80B54" constant="no"/>
<attribute name="OC_FARNELL" value="1849775  " constant="no"/>
<attribute name="OC_NEWARK" value="17T5178" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 50KOHM " constant="no"/>
</technology>
<technology name="2WSX80B55">
<attribute name="MPN" value="EVM2WSX80B55" constant="no"/>
<attribute name="OC_FARNELL" value="1849778  " constant="no"/>
<attribute name="OC_NEWARK" value="unknown" constant="no"/>
<attribute name="PACKAGE" value="EVM2W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500KOHM " constant="no"/>
</technology>
</technologies>
</device>
<device name="-3W" package="POT_EVM3W">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<technologies>
<technology name="3WSX80B12">
<attribute name="MPN" value="EVM3WSX80B12" constant="no"/>
<attribute name="OC_FARNELL" value="1849780  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0110" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100OHM " constant="no"/>
</technology>
<technology name="3WSX80B13">
<attribute name="MPN" value="EVM3WSX80B13" constant="no"/>
<attribute name="OC_FARNELL" value="1849783  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0111" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1KOHM " constant="no"/>
</technology>
<technology name="3WSX80B14">
<attribute name="MPN" value="EVM3WSX80B14" constant="no"/>
<attribute name="OC_FARNELL" value="1849787  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0112" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 10KOHM " constant="no"/>
</technology>
<technology name="3WSX80B15">
<attribute name="MPN" value="EVM3WSX80B15" constant="no"/>
<attribute name="OC_FARNELL" value="1849790  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0113" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100KOHM " constant="no"/>
</technology>
<technology name="3WSX80B16">
<attribute name="MPN" value="EVM3WSX80B16" constant="no"/>
<attribute name="OC_FARNELL" value="1849793  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0114" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1MOHM " constant="no"/>
</technology>
<technology name="3WSX80B22">
<attribute name="MPN" value="EVM3WSX80B22" constant="no"/>
<attribute name="OC_FARNELL" value="1849781  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0115" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200OHM " constant="no"/>
</technology>
<technology name="3WSX80B23">
<attribute name="MPN" value="EVM3WSX80B23" constant="no"/>
<attribute name="OC_FARNELL" value="1849784  " constant="no"/>
<attribute name="OC_NEWARK" value="93T1522" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 2KOHM " constant="no"/>
</technology>
<technology name="3WSX80B24">
<attribute name="MPN" value="EVM3WSX80B24" constant="no"/>
<attribute name="OC_FARNELL" value="1849788  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0116" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 20KOHM " constant="no"/>
</technology>
<technology name="3WSX80B25">
<attribute name="MPN" value="EVM3WSX80B25" constant="no"/>
<attribute name="OC_FARNELL" value="1849791  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0117" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200KOHM " constant="no"/>
</technology>
<technology name="3WSX80B52">
<attribute name="MPN" value="EVM3WSX80B52" constant="no"/>
<attribute name="OC_FARNELL" value="1849782  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0118" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500OHM " constant="no"/>
</technology>
<technology name="3WSX80B53">
<attribute name="MPN" value="EVM3WSX80B53" constant="no"/>
<attribute name="OC_FARNELL" value="1849786  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0119" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 5KOHM " constant="no"/>
</technology>
<technology name="3WSX80B54">
<attribute name="MPN" value="EVM3WSX80B54" constant="no"/>
<attribute name="OC_FARNELL" value="1849789  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0120" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 50KOHM " constant="no"/>
</technology>
<technology name="3WSX80B55">
<attribute name="MPN" value="EVM3WSX80B55" constant="no"/>
<attribute name="OC_FARNELL" value="1849792  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0121" constant="no"/>
<attribute name="PACKAGE" value="EVM3W" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500KOHM " constant="no"/>
</technology>
<technology name="3YSX50B12">
<attribute name="MPN" value="EVM3YSX50B12" constant="no"/>
<attribute name="OC_FARNELL" value="1849794  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0122" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y" constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100OHM " constant="no"/>
</technology>
<technology name="3YSX50B13">
<attribute name="MPN" value="EVM3YSX50B13" constant="no"/>
<attribute name="OC_FARNELL" value="1849798  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0123" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1KOHM " constant="no"/>
</technology>
<technology name="3YSX50B14">
<attribute name="MPN" value="EVM3YSX50B14" constant="no"/>
<attribute name="OC_FARNELL" value="1849801  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0124" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 10KOHM " constant="no"/>
</technology>
<technology name="3YSX50B15">
<attribute name="MPN" value="EVM3YSX50B15" constant="no"/>
<attribute name="OC_FARNELL" value="1849805  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0125" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100KOHM " constant="no"/>
</technology>
<technology name="3YSX50B16">
<attribute name="MPN" value="EVM3YSX50B16" constant="no"/>
<attribute name="OC_FARNELL" value="1849808  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0126" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1MOHM " constant="no"/>
</technology>
<technology name="3YSX50B22">
<attribute name="MPN" value="EVM3YSX50B22" constant="no"/>
<attribute name="OC_FARNELL" value="1849795  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0127" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200OHM " constant="no"/>
</technology>
<technology name="3YSX50B23">
<attribute name="MPN" value="EVM3YSX50B23" constant="no"/>
<attribute name="OC_FARNELL" value="1849799  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0128" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 2KOHM " constant="no"/>
</technology>
<technology name="3YSX50B24">
<attribute name="MPN" value="EVM3YSX50B24" constant="no"/>
<attribute name="OC_FARNELL" value="1849802  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0129" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 20KOHM " constant="no"/>
</technology>
<technology name="3YSX50B25">
<attribute name="MPN" value="EVM3YSX50B25" constant="no"/>
<attribute name="OC_FARNELL" value="1849806  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0130" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200KOHM " constant="no"/>
</technology>
<technology name="3YSX50B52">
<attribute name="MPN" value="EVM3YSX50B52" constant="no"/>
<attribute name="OC_FARNELL" value="1849796  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0131" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500OHM " constant="no"/>
</technology>
<technology name="3YSX50B53">
<attribute name="MPN" value="EVM3YSX50B53" constant="no"/>
<attribute name="OC_FARNELL" value="1849800  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0132" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 5KOHM " constant="no"/>
</technology>
<technology name="3YSX50B54">
<attribute name="MPN" value="EVM3YSX50B54" constant="no"/>
<attribute name="OC_FARNELL" value="1849804  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0133" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 50KOHM " constant="no"/>
</technology>
<technology name="3YSX50B55">
<attribute name="MPN" value="EVM3YSX50B55" constant="no"/>
<attribute name="OC_FARNELL" value="1849807  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0134" constant="no"/>
<attribute name="PACKAGE" value="EVM3Y " constant="no"/>
<attribute name="POWERRATING" value=" 0.15W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500KOHM " constant="no"/>
</technology>
</technologies>
</device>
<device name="-AA" package="POT_EVMAAS">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<technologies>
<technology name="AASA00B12">
<attribute name="MPN" value="EVMAASA00B12" constant="no"/>
<attribute name="OC_FARNELL" value="1849823  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0084" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100OHM " constant="no"/>
</technology>
<technology name="AASA00B13">
<attribute name="MPN" value="EVMAASA00B13" constant="no"/>
<attribute name="OC_FARNELL" value="1849826  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0085" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1KOHM " constant="no"/>
</technology>
<technology name="AASA00B14">
<attribute name="MPN" value="EVMAASA00B14" constant="no"/>
<attribute name="OC_FARNELL" value="1849830  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0086" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 10KOHM " constant="no"/>
</technology>
<technology name="AASA00B15">
<attribute name="MPN" value="EVMAASA00B15" constant="no"/>
<attribute name="OC_FARNELL" value="1849833  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0087" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 100KOHM " constant="no"/>
</technology>
<technology name="AASA00B16">
<attribute name="MPN" value="EVMAASA00B16" constant="no"/>
<attribute name="OC_FARNELL" value="1849836  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0088" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 1MOHM " constant="no"/>
</technology>
<technology name="AASA00B22">
<attribute name="MPN" value="EVMAASA00B22" constant="no"/>
<attribute name="OC_FARNELL" value="1849824  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0089" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200OHM " constant="no"/>
</technology>
<technology name="AASA00B23">
<attribute name="MPN" value="EVMAASA00B23" constant="no"/>
<attribute name="OC_FARNELL" value="1849828  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0090" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 2KOHM " constant="no"/>
</technology>
<technology name="AASA00B24">
<attribute name="MPN" value="EVMAASA00B24" constant="no"/>
<attribute name="OC_FARNELL" value="1849831  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0091" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 20KOHM " constant="no"/>
</technology>
<technology name="AASA00B25">
<attribute name="MPN" value="EVMAASA00B25" constant="no"/>
<attribute name="OC_FARNELL" value="1849834  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0092" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 200KOHM " constant="no"/>
</technology>
<technology name="AASA00B52">
<attribute name="MPN" value="EVMAASA00B52" constant="no"/>
<attribute name="OC_FARNELL" value="1849825  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0093" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500OHM " constant="no"/>
</technology>
<technology name="AASA00B53">
<attribute name="MPN" value="EVMAASA00B53" constant="no"/>
<attribute name="OC_FARNELL" value="1849829  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0094" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 5KOHM " constant="no"/>
</technology>
<technology name="AASA00B54">
<attribute name="MPN" value="EVMAASA00B54" constant="no"/>
<attribute name="OC_FARNELL" value="1849832  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0095" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 50KOHM " constant="no"/>
</technology>
<technology name="AASA00B55">
<attribute name="MPN" value="EVMAASA00B55" constant="no"/>
<attribute name="OC_FARNELL" value="1849835  " constant="no"/>
<attribute name="OC_NEWARK" value="48W0096" constant="no"/>
<attribute name="PACKAGE" value="EVMAA" constant="no"/>
<attribute name="POWERRATING" value=" 0.3W " constant="no"/>
<attribute name="SUPPLIER" value="PANASONIC" constant="no"/>
<attribute name="TOLERANCE" value="±25 %" constant="no"/>
<attribute name="VALUE" value=" 500KOHM " constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="jumper">
<description>&lt;b&gt;Jumpers&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="SJ">
<description>&lt;b&gt;Solder jumper&lt;/b&gt;</description>
<wire x1="1.397" y1="-1.016" x2="-1.397" y2="-1.016" width="0.1524" layer="21"/>
<wire x1="1.397" y1="1.016" x2="1.651" y2="0.762" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.651" y1="0.762" x2="-1.397" y2="1.016" width="0.1524" layer="21" curve="-90"/>
<wire x1="-1.651" y1="-0.762" x2="-1.397" y2="-1.016" width="0.1524" layer="21" curve="90"/>
<wire x1="1.397" y1="-1.016" x2="1.651" y2="-0.762" width="0.1524" layer="21" curve="90"/>
<wire x1="1.651" y1="-0.762" x2="1.651" y2="0.762" width="0.1524" layer="21"/>
<wire x1="-1.651" y1="-0.762" x2="-1.651" y2="0.762" width="0.1524" layer="21"/>
<wire x1="-1.397" y1="1.016" x2="1.397" y2="1.016" width="0.1524" layer="21"/>
<wire x1="1.016" y1="0" x2="1.524" y2="0" width="0.1524" layer="51"/>
<wire x1="-1.016" y1="0" x2="-1.524" y2="0" width="0.1524" layer="51"/>
<wire x1="-0.254" y1="-0.127" x2="-0.254" y2="0.127" width="1.27" layer="51" curve="-180" cap="flat"/>
<wire x1="0.254" y1="0.127" x2="0.254" y2="-0.127" width="1.27" layer="51" curve="-180" cap="flat"/>
<smd name="1" x="-0.762" y="0" dx="1.1684" dy="1.6002" layer="1"/>
<smd name="2" x="0.762" y="0" dx="1.1684" dy="1.6002" layer="1"/>
<text x="-1.651" y="1.143" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="0.4001" y="0" size="0.02" layer="27">&gt;VALUE</text>
<rectangle x1="-0.0762" y1="-0.9144" x2="0.0762" y2="0.9144" layer="29"/>
</package>
<package name="SJW">
<description>&lt;b&gt;Solder jumper&lt;/b&gt;</description>
<wire x1="1.905" y1="-1.524" x2="-1.905" y2="-1.524" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.524" x2="2.159" y2="1.27" width="0.1524" layer="21" curve="-90"/>
<wire x1="-2.159" y1="1.27" x2="-1.905" y2="1.524" width="0.1524" layer="21" curve="-90"/>
<wire x1="-2.159" y1="-1.27" x2="-1.905" y2="-1.524" width="0.1524" layer="21" curve="90"/>
<wire x1="1.905" y1="-1.524" x2="2.159" y2="-1.27" width="0.1524" layer="21" curve="90"/>
<wire x1="2.159" y1="-1.27" x2="2.159" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.159" y1="-1.27" x2="-2.159" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.524" x2="1.905" y2="1.524" width="0.1524" layer="21"/>
<wire x1="0.762" y1="0.762" x2="0.762" y2="-0.762" width="0.1524" layer="51"/>
<wire x1="-0.762" y1="0.762" x2="-0.762" y2="-0.762" width="0.1524" layer="51"/>
<wire x1="1.524" y1="0" x2="2.032" y2="0" width="0.1524" layer="51"/>
<wire x1="-1.524" y1="0" x2="-2.032" y2="0" width="0.1524" layer="51"/>
<wire x1="0.762" y1="0.762" x2="0.762" y2="-0.762" width="0.1524" layer="51" curve="-180"/>
<wire x1="-0.762" y1="0.762" x2="-0.762" y2="-0.762" width="0.1524" layer="51" curve="180"/>
<smd name="1" x="-1.27" y="0" dx="1.27" dy="2.54" layer="1"/>
<smd name="2" x="1.27" y="0" dx="1.27" dy="2.54" layer="1"/>
<text x="-2.159" y="1.778" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="1" y="0" size="0.02" layer="27">&gt;VALUE</text>
<rectangle x1="0.762" y1="-0.762" x2="1.016" y2="0.762" layer="51"/>
<rectangle x1="1.016" y1="-0.635" x2="1.27" y2="0.635" layer="51"/>
<rectangle x1="1.27" y1="-0.508" x2="1.397" y2="0.508" layer="51"/>
<rectangle x1="1.397" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.016" y1="-0.762" x2="-0.762" y2="0.762" layer="51"/>
<rectangle x1="-1.27" y1="-0.635" x2="-1.016" y2="0.635" layer="51"/>
<rectangle x1="-1.397" y1="-0.508" x2="-1.27" y2="0.508" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.397" y2="0.254" layer="51"/>
<rectangle x1="0.9652" y1="-0.7112" x2="1.0922" y2="-0.5842" layer="51"/>
<rectangle x1="1.3462" y1="-0.3556" x2="1.4732" y2="-0.2286" layer="51"/>
<rectangle x1="1.3462" y1="0.2032" x2="1.4732" y2="0.3302" layer="51"/>
<rectangle x1="0.9652" y1="0.5842" x2="1.0922" y2="0.7112" layer="51"/>
<rectangle x1="-1.0922" y1="-0.7112" x2="-0.9652" y2="-0.5842" layer="51"/>
<rectangle x1="-1.4478" y1="-0.3302" x2="-1.3208" y2="-0.2032" layer="51"/>
<rectangle x1="-1.4732" y1="0.2032" x2="-1.3462" y2="0.3302" layer="51"/>
<rectangle x1="-1.1176" y1="0.5842" x2="-0.9906" y2="0.7112" layer="51"/>
</package>
</packages>
<symbols>
<symbol name="SJ">
<wire x1="0.381" y1="0.635" x2="0.381" y2="-0.635" width="1.27" layer="94" curve="-180" cap="flat"/>
<wire x1="-0.381" y1="-0.635" x2="-0.381" y2="0.635" width="1.27" layer="94" curve="-180" cap="flat"/>
<wire x1="2.54" y1="0" x2="1.651" y2="0" width="0.1524" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.651" y2="0" width="0.1524" layer="94"/>
<text x="-2.54" y="2.54" size="1.778" layer="95">&gt;NAME</text>
<text x="-2.54" y="-3.81" size="1.778" layer="96">&gt;VALUE</text>
<pin name="2" x="5.08" y="0" visible="pad" length="short" direction="pas" swaplevel="1" rot="R180"/>
<pin name="1" x="-5.08" y="0" visible="pad" length="short" direction="pas" swaplevel="1"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SJ" prefix="SJ" uservalue="yes">
<description>SMD solder &lt;b&gt;JUMPER&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="SJ" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SJ">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="W" package="SJW">
<connects>
<connect gate="1" pin="1" pad="1"/>
<connect gate="1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="MAIN" library="con-lstb" deviceset="MA08-2" device=""/>
<part name="LCD8X2" library="con-lstb" deviceset="MA08-2" device="" value="LCD"/>
<part name="FTDI" library="con-lstb" deviceset="MA06-1" device=""/>
<part name="P+1" library="supply1" deviceset="VCC" device=""/>
<part name="GND1" library="supply1" deviceset="GND" device=""/>
<part name="R1" library="Panasonic-Trimmer_Potentiometers" deviceset="EVM" device="-3W" technology="3YSX50B14" value=" 10KOHM "/>
<part name="SJ1" library="jumper" deviceset="SJ" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<text x="-22.86" y="91.44" size="1.778" layer="91">LCD board</text>
</plain>
<instances>
<instance part="MAIN" gate="G$1" x="17.78" y="58.42" rot="R180"/>
<instance part="LCD8X2" gate="G$1" x="58.42" y="58.42" rot="R180"/>
<instance part="FTDI" gate="1" x="-22.86" y="58.42" rot="MR180"/>
<instance part="P+1" gate="VCC" x="7.62" y="88.9"/>
<instance part="GND1" gate="1" x="7.62" y="35.56"/>
<instance part="R1" gate="A" x="33.02" y="86.36" rot="R270"/>
<instance part="SJ1" gate="1" x="38.1" y="63.5" rot="R180"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$2" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="16"/>
<pinref part="LCD8X2" gate="G$1" pin="15"/>
<wire x1="25.4" y1="48.26" x2="50.8" y2="48.26" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="14"/>
<pinref part="LCD8X2" gate="G$1" pin="13"/>
<wire x1="25.4" y1="50.8" x2="50.8" y2="50.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="12"/>
<pinref part="LCD8X2" gate="G$1" pin="11"/>
<wire x1="25.4" y1="53.34" x2="50.8" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="VCC" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="1"/>
<wire x1="10.16" y1="66.04" x2="7.62" y2="66.04" width="0.1524" layer="91"/>
<wire x1="7.62" y1="66.04" x2="7.62" y2="86.36" width="0.1524" layer="91"/>
<pinref part="P+1" gate="VCC" pin="VCC"/>
<pinref part="LCD8X2" gate="G$1" pin="2"/>
<wire x1="66.04" y1="66.04" x2="68.58" y2="66.04" width="0.1524" layer="91"/>
<wire x1="68.58" y1="66.04" x2="68.58" y2="86.36" width="0.1524" layer="91"/>
<junction x="7.62" y="86.36"/>
<wire x1="68.58" y1="86.36" x2="33.02" y2="86.36" width="0.1524" layer="91"/>
<pinref part="R1" gate="A" pin="1"/>
<junction x="33.02" y="86.36"/>
<wire x1="33.02" y1="86.36" x2="7.62" y2="86.36" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="2"/>
<wire x1="25.4" y1="66.04" x2="33.02" y2="66.04" width="0.1524" layer="91"/>
<wire x1="33.02" y1="66.04" x2="33.02" y2="63.5" width="0.1524" layer="91"/>
<wire x1="33.02" y1="63.5" x2="33.02" y2="38.1" width="0.1524" layer="91"/>
<wire x1="33.02" y1="38.1" x2="7.62" y2="38.1" width="0.1524" layer="91"/>
<wire x1="7.62" y1="38.1" x2="7.62" y2="48.26" width="0.1524" layer="91"/>
<pinref part="MAIN" gate="G$1" pin="15"/>
<wire x1="7.62" y1="48.26" x2="10.16" y2="48.26" width="0.1524" layer="91"/>
<pinref part="GND1" gate="1" pin="GND"/>
<junction x="7.62" y="38.1"/>
<pinref part="LCD8X2" gate="G$1" pin="16"/>
<wire x1="66.04" y1="48.26" x2="68.58" y2="48.26" width="0.1524" layer="91"/>
<wire x1="68.58" y1="48.26" x2="68.58" y2="38.1" width="0.1524" layer="91"/>
<wire x1="68.58" y1="38.1" x2="48.26" y2="38.1" width="0.1524" layer="91"/>
<junction x="33.02" y="38.1"/>
<pinref part="LCD8X2" gate="G$1" pin="1"/>
<wire x1="48.26" y1="38.1" x2="33.02" y2="38.1" width="0.1524" layer="91"/>
<wire x1="50.8" y1="66.04" x2="33.02" y2="66.04" width="0.1524" layer="91"/>
<junction x="33.02" y="66.04"/>
<pinref part="LCD8X2" gate="G$1" pin="9"/>
<wire x1="50.8" y1="55.88" x2="48.26" y2="55.88" width="0.1524" layer="91"/>
<wire x1="48.26" y1="55.88" x2="48.26" y2="58.42" width="0.1524" layer="91"/>
<pinref part="LCD8X2" gate="G$1" pin="7"/>
<wire x1="48.26" y1="58.42" x2="50.8" y2="58.42" width="0.1524" layer="91"/>
<wire x1="48.26" y1="55.88" x2="48.26" y2="38.1" width="0.1524" layer="91"/>
<junction x="48.26" y="55.88"/>
<junction x="48.26" y="38.1"/>
<pinref part="LCD8X2" gate="G$1" pin="10"/>
<wire x1="66.04" y1="55.88" x2="68.58" y2="55.88" width="0.1524" layer="91"/>
<wire x1="68.58" y1="55.88" x2="68.58" y2="58.42" width="0.1524" layer="91"/>
<pinref part="LCD8X2" gate="G$1" pin="8"/>
<wire x1="68.58" y1="58.42" x2="66.04" y2="58.42" width="0.1524" layer="91"/>
<wire x1="68.58" y1="55.88" x2="68.58" y2="48.26" width="0.1524" layer="91"/>
<junction x="68.58" y="55.88"/>
<junction x="68.58" y="48.26"/>
<pinref part="R1" gate="A" pin="3"/>
<wire x1="-10.16" y1="66.04" x2="-10.16" y2="38.1" width="0.1524" layer="91"/>
<wire x1="-10.16" y1="38.1" x2="7.62" y2="38.1" width="0.1524" layer="91"/>
<pinref part="FTDI" gate="1" pin="1"/>
<wire x1="-15.24" y1="66.04" x2="-10.16" y2="66.04" width="0.1524" layer="91"/>
<pinref part="SJ1" gate="1" pin="2"/>
<junction x="33.02" y="63.5"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="13"/>
<wire x1="10.16" y1="50.8" x2="0" y2="50.8" width="0.1524" layer="91"/>
<wire x1="0" y1="50.8" x2="0" y2="30.48" width="0.1524" layer="91"/>
<wire x1="0" y1="30.48" x2="73.66" y2="30.48" width="0.1524" layer="91"/>
<wire x1="73.66" y1="30.48" x2="73.66" y2="50.8" width="0.1524" layer="91"/>
<pinref part="LCD8X2" gate="G$1" pin="14"/>
<wire x1="73.66" y1="50.8" x2="66.04" y2="50.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="LCD8X2" gate="G$1" pin="12"/>
<wire x1="66.04" y1="53.34" x2="76.2" y2="53.34" width="0.1524" layer="91"/>
<wire x1="76.2" y1="53.34" x2="76.2" y2="27.94" width="0.1524" layer="91"/>
<wire x1="76.2" y1="27.94" x2="-2.54" y2="27.94" width="0.1524" layer="91"/>
<wire x1="-2.54" y1="27.94" x2="-2.54" y2="53.34" width="0.1524" layer="91"/>
<pinref part="MAIN" gate="G$1" pin="11"/>
<wire x1="-2.54" y1="53.34" x2="10.16" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="LCD8X2" gate="G$1" pin="5"/>
<pinref part="MAIN" gate="G$1" pin="6"/>
<wire x1="50.8" y1="60.96" x2="25.4" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<wire x1="-2.54" y1="60.96" x2="-2.54" y2="93.98" width="0.1524" layer="91"/>
<wire x1="-2.54" y1="93.98" x2="76.2" y2="93.98" width="0.1524" layer="91"/>
<wire x1="76.2" y1="93.98" x2="76.2" y2="60.96" width="0.1524" layer="91"/>
<pinref part="LCD8X2" gate="G$1" pin="6"/>
<wire x1="76.2" y1="60.96" x2="66.04" y2="60.96" width="0.1524" layer="91"/>
<pinref part="MAIN" gate="G$1" pin="5"/>
<wire x1="-2.54" y1="60.96" x2="10.16" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="LCD8X2" gate="G$1" pin="4"/>
<wire x1="66.04" y1="63.5" x2="73.66" y2="63.5" width="0.1524" layer="91"/>
<wire x1="73.66" y1="63.5" x2="73.66" y2="91.44" width="0.1524" layer="91"/>
<wire x1="73.66" y1="91.44" x2="0" y2="91.44" width="0.1524" layer="91"/>
<wire x1="0" y1="91.44" x2="0" y2="63.5" width="0.1524" layer="91"/>
<pinref part="MAIN" gate="G$1" pin="3"/>
<wire x1="0" y1="63.5" x2="10.16" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="LCD8X2" gate="G$1" pin="3"/>
<pinref part="R1" gate="A" pin="2"/>
<wire x1="40.64" y1="76.2" x2="43.18" y2="76.2" width="0.1524" layer="91"/>
<wire x1="43.18" y1="76.2" x2="43.18" y2="63.5" width="0.1524" layer="91"/>
<wire x1="43.18" y1="63.5" x2="50.8" y2="63.5" width="0.1524" layer="91"/>
<pinref part="SJ1" gate="1" pin="1"/>
<junction x="43.18" y="63.5"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<wire x1="27.94" y1="58.42" x2="27.94" y2="40.64" width="0.1524" layer="91"/>
<wire x1="27.94" y1="40.64" x2="-7.62" y2="40.64" width="0.1524" layer="91"/>
<wire x1="-7.62" y1="40.64" x2="-7.62" y2="58.42" width="0.1524" layer="91"/>
<pinref part="FTDI" gate="1" pin="4"/>
<wire x1="-15.24" y1="58.42" x2="-7.62" y2="58.42" width="0.1524" layer="91"/>
<pinref part="MAIN" gate="G$1" pin="8"/>
<wire x1="25.4" y1="58.42" x2="27.94" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="MAIN" gate="G$1" pin="7"/>
<wire x1="10.16" y1="58.42" x2="-2.54" y2="58.42" width="0.1524" layer="91"/>
<wire x1="-2.54" y1="58.42" x2="-2.54" y2="55.88" width="0.1524" layer="91"/>
<pinref part="FTDI" gate="1" pin="5"/>
<wire x1="-2.54" y1="55.88" x2="-15.24" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>

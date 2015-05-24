<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="7.2.0">
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
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="118" name="Rect_Pads" color="4" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="7Segments">
<packages>
<package name="HDSP-077X">
<pad name="GND" x="5.08" y="0" drill="0.8"/>
<pad name="VCC" x="2.54" y="0" drill="0.8"/>
<pad name="IN1" x="0" y="0" drill="0.8"/>
<pad name="IN8" x="5.08" y="-15.24" drill="0.8"/>
<pad name="IN4" x="2.54" y="-15.24" drill="0.8"/>
<polygon width="0.127" layer="21">
<vertex x="-1.27" y="0"/>
<vertex x="8.89" y="0"/>
<vertex x="8.89" y="-15.24"/>
<vertex x="-1.27" y="-15.24"/>
</polygon>
<pad name="DP" x="7.62" y="-15.24" drill="0.8"/>
<pad name="LE" x="7.62" y="0" drill="0.8"/>
<pad name="IN2" x="0" y="-15.24" drill="0.8" shape="square"/>
</package>
</packages>
<symbols>
<symbol name="HDSP-077X-LATEST">
<pin name="LE" x="7.62" y="0" visible="pad" length="middle" swaplevel="5" rot="R270"/>
<pin name="GND" x="5.08" y="0" visible="pad" length="middle" swaplevel="6" rot="R270"/>
<pin name="VCC" x="2.54" y="0" visible="pad" length="middle" swaplevel="7" rot="R270"/>
<pin name="IN1" x="0" y="0" visible="pad" length="middle" swaplevel="8" rot="R270"/>
<pin name="DP" x="7.62" y="-15.24" visible="pad" length="middle" swaplevel="4" rot="R90"/>
<pin name="IN8" x="5.08" y="-15.24" visible="pad" length="middle" swaplevel="3" rot="R90"/>
<pin name="IN4" x="2.54" y="-15.24" visible="pad" length="middle" swaplevel="2" rot="R90"/>
<pin name="IN2" x="0" y="-15.24" visible="pad" length="middle" swaplevel="1" rot="R90"/>
<wire x1="-2.54" y1="0" x2="10.16" y2="0" width="0.254" layer="94"/>
<wire x1="10.16" y1="0" x2="10.16" y2="-15.24" width="0.254" layer="94"/>
<wire x1="10.16" y1="-15.24" x2="-2.54" y2="-15.24" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-15.24" x2="-2.54" y2="0" width="0.254" layer="94"/>
<text x="1.524" y="-9.398" size="1.4224" layer="94">HDSP
077x</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="HDSP-077X">
<gates>
<gate name="G$1" symbol="HDSP-077X-LATEST" x="-15.24" y="20.32"/>
</gates>
<devices>
<device name="" package="HDSP-077X">
<connects>
<connect gate="G$1" pin="DP" pad="DP"/>
<connect gate="G$1" pin="GND" pad="GND"/>
<connect gate="G$1" pin="IN1" pad="IN1"/>
<connect gate="G$1" pin="IN2" pad="IN2"/>
<connect gate="G$1" pin="IN4" pad="IN4"/>
<connect gate="G$1" pin="IN8" pad="IN8"/>
<connect gate="G$1" pin="LE" pad="LE"/>
<connect gate="G$1" pin="VCC" pad="VCC"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="dab5267-fluxos">
<packages>
<package name="BEAGLE_EXPANSION">
<pad name="J3_3" x="0" y="2.54" drill="1" diameter="1.625"/>
<pad name="J3_5" x="0" y="5.08" drill="1" diameter="1.625"/>
<pad name="J3_7" x="0" y="7.62" drill="1" diameter="1.625"/>
<pad name="J3_9" x="0" y="10.16" drill="1" diameter="1.625"/>
<pad name="J3_11" x="0" y="12.7" drill="1" diameter="1.625"/>
<pad name="J3_13" x="0" y="15.24" drill="1" diameter="1.625"/>
<pad name="J3_15" x="0" y="17.78" drill="1" diameter="1.625"/>
<pad name="J3_17" x="0" y="20.32" drill="1" diameter="1.625"/>
<pad name="J3_19" x="0" y="22.86" drill="1" diameter="1.625"/>
<pad name="J3_21" x="0" y="25.4" drill="1" diameter="1.625"/>
<pad name="J3_23" x="0" y="27.94" drill="1" diameter="1.625"/>
<pad name="J3_25" x="0" y="30.48" drill="1" diameter="1.625"/>
<pad name="J3_27" x="0" y="33.02" drill="1" diameter="1.625"/>
<pad name="J3_2" x="2.54" y="0" drill="1" diameter="1.625"/>
<pad name="J3_4" x="2.54" y="2.54" drill="1" diameter="1.625"/>
<pad name="J3_6" x="2.54" y="5.08" drill="1" diameter="1.625"/>
<pad name="J3_8" x="2.54" y="7.62" drill="1" diameter="1.625"/>
<pad name="J3_10" x="2.54" y="10.16" drill="1" diameter="1.625"/>
<pad name="J3_12" x="2.54" y="12.7" drill="1" diameter="1.625"/>
<pad name="J3_14" x="2.54" y="15.24" drill="1" diameter="1.625"/>
<pad name="J3_16" x="2.54" y="17.78" drill="1" diameter="1.625"/>
<pad name="J3_18" x="2.54" y="20.32" drill="1" diameter="1.625"/>
<pad name="J3_20" x="2.54" y="22.86" drill="1" diameter="1.625"/>
<pad name="J3_22" x="2.54" y="25.4" drill="1" diameter="1.625"/>
<pad name="J3_24" x="2.54" y="27.94" drill="1" diameter="1.625"/>
<pad name="J3_26" x="2.54" y="30.48" drill="1" diameter="1.625"/>
<pad name="J3_28" x="2.54" y="33.02" drill="1" diameter="1.625"/>
<rectangle x1="-3.81" y1="58.42" x2="2.54" y2="64.77" layer="17"/>
<rectangle x1="-3.81" y1="-10.16" x2="2.54" y2="-3.81" layer="17"/>
<pad name="J3_1" x="0" y="0" drill="1" diameter="1.6764" shape="square"/>
<pad name="P$69" x="-0.635" y="-6.985" drill="3.2" shape="square"/>
<pad name="P$72" x="-0.635" y="61.595" drill="3.2" shape="square"/>
<wire x1="-1.27" y1="-1.27" x2="1.27" y2="-1.27" width="0.127" layer="21"/>
<wire x1="1.27" y1="-1.27" x2="3.81" y2="-1.27" width="0.127" layer="21"/>
<wire x1="3.81" y1="-1.27" x2="3.81" y2="34.29" width="0.127" layer="21"/>
<wire x1="3.81" y1="34.29" x2="-1.27" y2="34.29" width="0.127" layer="21"/>
<wire x1="-1.27" y1="34.29" x2="-1.27" y2="1.27" width="0.127" layer="21"/>
<wire x1="-1.27" y1="1.27" x2="-1.27" y2="-1.27" width="0.127" layer="21"/>
<text x="-1.905" y="0" size="0.8128" layer="21" rot="R90" align="center">J3</text>
<text x="0" y="-2.032" size="0.8128" layer="21" align="center">1</text>
<text x="2.667" y="-2.032" size="0.8128" layer="21" align="center">2</text>
<text x="0" y="34.925" size="0.8128" layer="21" align="center">27</text>
<text x="2.54" y="34.925" size="0.8128" layer="21" align="center">28</text>
<wire x1="1.27" y1="-1.27" x2="1.27" y2="1.143" width="0.127" layer="21"/>
<wire x1="1.27" y1="1.143" x2="1.27" y2="1.27" width="0.127" layer="21"/>
<wire x1="1.27" y1="1.27" x2="-1.27" y2="1.27" width="0.127" layer="21"/>
</package>
</packages>
<symbols>
<symbol name=".1IN_14X2">
<wire x1="3.81" y1="-20.32" x2="-3.81" y2="-20.32" width="0.4064" layer="94"/>
<wire x1="-1.27" y1="-12.7" x2="-2.54" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-15.24" x2="-2.54" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-17.78" x2="-2.54" y2="-17.78" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-12.7" x2="1.27" y2="-12.7" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-15.24" x2="1.27" y2="-15.24" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-17.78" x2="1.27" y2="-17.78" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-7.62" x2="-2.54" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-10.16" x2="-2.54" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-7.62" x2="1.27" y2="-7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-10.16" x2="1.27" y2="-10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="0" x2="-2.54" y2="0" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-2.54" x2="-2.54" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="-5.08" x2="-2.54" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="0" x2="1.27" y2="0" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-2.54" x2="1.27" y2="-2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="-5.08" x2="1.27" y2="-5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="5.08" x2="-2.54" y2="5.08" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="2.54" x2="-2.54" y2="2.54" width="0.6096" layer="94"/>
<wire x1="2.54" y1="5.08" x2="1.27" y2="5.08" width="0.6096" layer="94"/>
<wire x1="2.54" y1="2.54" x2="1.27" y2="2.54" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="12.7" x2="-2.54" y2="12.7" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="10.16" x2="-2.54" y2="10.16" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="7.62" x2="-2.54" y2="7.62" width="0.6096" layer="94"/>
<wire x1="2.54" y1="12.7" x2="1.27" y2="12.7" width="0.6096" layer="94"/>
<wire x1="2.54" y1="10.16" x2="1.27" y2="10.16" width="0.6096" layer="94"/>
<wire x1="2.54" y1="7.62" x2="1.27" y2="7.62" width="0.6096" layer="94"/>
<wire x1="-1.27" y1="15.24" x2="-2.54" y2="15.24" width="0.6096" layer="94"/>
<wire x1="2.54" y1="15.24" x2="1.27" y2="15.24" width="0.6096" layer="94"/>
<wire x1="-3.81" y1="17.78" x2="-3.81" y2="-20.32" width="0.4064" layer="94"/>
<wire x1="3.81" y1="-20.32" x2="3.81" y2="17.78" width="0.4064" layer="94"/>
<wire x1="-3.81" y1="17.78" x2="3.81" y2="17.78" width="0.4064" layer="94"/>
<text x="-1.524" y="18.542" size="1.778" layer="95">&gt;NAME</text>
<pin name="1" x="-7.62" y="-17.78" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="3" x="-7.62" y="-15.24" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="5" x="-7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="2" x="7.62" y="-17.78" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="4" x="7.62" y="-15.24" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="6" x="7.62" y="-12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="7" x="-7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="9" x="-7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="8" x="7.62" y="-10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="10" x="7.62" y="-7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="11" x="-7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="13" x="-7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="15" x="-7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="12" x="7.62" y="-5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="14" x="7.62" y="-2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="16" x="7.62" y="0" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="17" x="-7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="19" x="-7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="18" x="7.62" y="2.54" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="20" x="7.62" y="5.08" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="21" x="-7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="23" x="-7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="25" x="-7.62" y="12.7" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="22" x="7.62" y="7.62" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="24" x="7.62" y="10.16" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="26" x="7.62" y="12.7" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
<pin name="27" x="-7.62" y="15.24" visible="pad" length="middle" direction="pas" swaplevel="1"/>
<pin name="28" x="7.62" y="15.24" visible="pad" length="middle" direction="pas" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="BEAGLE_EXPANSION" prefix="BB">
<description>&lt;h2&gt;BeagleBoard C4 Expansion&lt;/h2&gt;
&lt;p&gt;Provides pinouts for J3, J4, and J5.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;J3&lt;/b&gt; is the main expansion header.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;J4&lt;/b&gt; is the first LCD connector.&lt;/p&gt;
&lt;p&gt;&lt;b&gt;J5&lt;/b&gt; is the second LCD connector.&lt;/p&gt;</description>
<gates>
<gate name="J3" symbol=".1IN_14X2" x="0" y="-22.86"/>
</gates>
<devices>
<device name="" package="BEAGLE_EXPANSION">
<connects>
<connect gate="J3" pin="1" pad="J3_1"/>
<connect gate="J3" pin="10" pad="J3_10"/>
<connect gate="J3" pin="11" pad="J3_11"/>
<connect gate="J3" pin="12" pad="J3_12"/>
<connect gate="J3" pin="13" pad="J3_13"/>
<connect gate="J3" pin="14" pad="J3_14"/>
<connect gate="J3" pin="15" pad="J3_15"/>
<connect gate="J3" pin="16" pad="J3_16"/>
<connect gate="J3" pin="17" pad="J3_17"/>
<connect gate="J3" pin="18" pad="J3_18"/>
<connect gate="J3" pin="19" pad="J3_19"/>
<connect gate="J3" pin="2" pad="J3_2"/>
<connect gate="J3" pin="20" pad="J3_20"/>
<connect gate="J3" pin="21" pad="J3_21"/>
<connect gate="J3" pin="22" pad="J3_22"/>
<connect gate="J3" pin="23" pad="J3_23"/>
<connect gate="J3" pin="24" pad="J3_24"/>
<connect gate="J3" pin="25" pad="J3_25"/>
<connect gate="J3" pin="26" pad="J3_26"/>
<connect gate="J3" pin="27" pad="J3_27"/>
<connect gate="J3" pin="28" pad="J3_28"/>
<connect gate="J3" pin="3" pad="J3_3"/>
<connect gate="J3" pin="4" pad="J3_4"/>
<connect gate="J3" pin="5" pad="J3_5"/>
<connect gate="J3" pin="6" pad="J3_6"/>
<connect gate="J3" pin="7" pad="J3_7"/>
<connect gate="J3" pin="8" pad="J3_8"/>
<connect gate="J3" pin="9" pad="J3_9"/>
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
<part name="BB1" library="dab5267-fluxos" deviceset="BEAGLE_EXPANSION" device=""/>
<part name="U$1" library="7Segments" deviceset="HDSP-077X" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="BB1" gate="J3" x="27.94" y="43.18"/>
<instance part="U$1" gate="G$1" x="33.02" y="101.6" rot="R270"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$2" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="VCC"/>
<wire x1="33.02" y1="99.06" x2="58.42" y2="99.06" width="0.1524" layer="91"/>
<pinref part="BB1" gate="J3" pin="2"/>
<wire x1="58.42" y1="99.06" x2="58.42" y2="25.4" width="0.1524" layer="91"/>
<wire x1="58.42" y1="25.4" x2="35.56" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="BB1" gate="J3" pin="28"/>
<wire x1="53.34" y1="58.42" x2="35.56" y2="58.42" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="GND"/>
<wire x1="33.02" y1="96.52" x2="53.34" y2="96.52" width="0.1524" layer="91"/>
<wire x1="53.34" y1="96.52" x2="53.34" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="DP"/>
<wire x1="17.78" y1="93.98" x2="10.16" y2="93.98" width="0.1524" layer="91"/>
<pinref part="BB1" gate="J3" pin="15"/>
<wire x1="10.16" y1="93.98" x2="10.16" y2="43.18" width="0.1524" layer="91"/>
<wire x1="10.16" y1="43.18" x2="20.32" y2="43.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="LE"/>
<wire x1="33.02" y1="93.98" x2="50.8" y2="93.98" width="0.1524" layer="91"/>
<wire x1="50.8" y1="93.98" x2="50.8" y2="66.04" width="0.1524" layer="91"/>
<wire x1="50.8" y1="66.04" x2="12.7" y2="66.04" width="0.1524" layer="91"/>
<pinref part="BB1" gate="J3" pin="17"/>
<wire x1="12.7" y1="66.04" x2="12.7" y2="45.72" width="0.1524" layer="91"/>
<wire x1="12.7" y1="45.72" x2="20.32" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="IN8"/>
<wire x1="17.78" y1="96.52" x2="7.62" y2="96.52" width="0.1524" layer="91"/>
<pinref part="BB1" gate="J3" pin="13"/>
<wire x1="7.62" y1="96.52" x2="7.62" y2="40.64" width="0.1524" layer="91"/>
<wire x1="7.62" y1="40.64" x2="20.32" y2="40.64" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="IN4"/>
<wire x1="17.78" y1="99.06" x2="5.08" y2="99.06" width="0.1524" layer="91"/>
<pinref part="BB1" gate="J3" pin="11"/>
<wire x1="5.08" y1="99.06" x2="5.08" y2="38.1" width="0.1524" layer="91"/>
<wire x1="5.08" y1="38.1" x2="20.32" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="IN1"/>
<wire x1="33.02" y1="101.6" x2="38.1" y2="101.6" width="0.1524" layer="91"/>
<wire x1="38.1" y1="101.6" x2="38.1" y2="109.22" width="0.1524" layer="91"/>
<wire x1="38.1" y1="109.22" x2="-2.54" y2="109.22" width="0.1524" layer="91"/>
<pinref part="BB1" gate="J3" pin="7"/>
<wire x1="-2.54" y1="109.22" x2="-2.54" y2="33.02" width="0.1524" layer="91"/>
<wire x1="-2.54" y1="33.02" x2="20.32" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="BB1" gate="J3" pin="9"/>
<wire x1="20.32" y1="35.56" x2="2.54" y2="35.56" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="IN2"/>
<wire x1="2.54" y1="35.56" x2="2.54" y2="101.6" width="0.1524" layer="91"/>
<wire x1="2.54" y1="101.6" x2="17.78" y2="101.6" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="6.3" minversion="6.2.2" severity="warning">
Since Version 6.2.2 text objects can contain more than one line,
which will not be processed correctly with this version.
</note>
</compatibility>
</eagle>

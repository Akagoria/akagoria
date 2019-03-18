<?xml version="1.0" encoding="UTF-8"?>
<tileset name="Biomes" tilewidth="32" tileheight="32" tilecount="2025" columns="45" spacing="2" margin="1">
<image source="biomes.png" width="1530" height="1530"/>
<terraintypes>
	<terrain name="DeepWater" tile="135"/>
	<terrain name="Water" tile="47"/>
	<terrain name="ShallowWater" tile="51"/>
	<terrain name="Shore" tile="55"/>
	<terrain name="Sand" tile="59"/>
	<terrain name="SubtropicalDesert" tile="63"/>
	<terrain name="TemperateDesert" tile="79"/>
	<terrain name="TropicalSeasonalForest" tile="71"/>
	<terrain name="TropicalRainForest" tile="75"/>
	<terrain name="TemperateRainForest" tile="243"/>
	<terrain name="TemperateDeciduousForest" tile="227"/>
	<terrain name="Grassland" tile="67"/>
	<terrain name="Shrubland" tile="231"/>
	<terrain name="Taiga" tile="267"/>
	<terrain name="Scorched" tile="259"/>
	<terrain name="Bare" tile="255"/>
	<terrain name="Tundra" tile="423"/>
	<terrain name="Snow" tile="419"/>
	<terrain name="Unreachable" tile="1395"/>
	<terrain name="Road" tile="1399"/>
	<terrain name="Track" tile="1403"/>
	<terrain name="River" tile="535"/>
</terraintypes>
<tile id="0" terrain="0,0,1,0"/>
<tile id="1" terrain="0,1,0,1"/>
<tile id="2" terrain="1,0,1,1"/>
<tile id="3" terrain="0,0,1,1"/>
<tile id="4" terrain="1,1,2,1"/>
<tile id="5" terrain="1,2,1,2"/>
<tile id="6" terrain="2,1,2,2"/>
<tile id="7" terrain="1,1,2,2"/>
<tile id="8" terrain="2,2,3,2"/>
<tile id="9" terrain="2,3,2,3"/>
<tile id="10" terrain="3,2,3,3"/>
<tile id="11" terrain="2,2,3,3"/>
<tile id="12" terrain="3,3,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="13" terrain="3,4,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UD"/>
	</properties>
</tile>
<tile id="14" terrain="4,3,4,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="15" terrain="3,3,4,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LR"/>
	</properties>
</tile>
<tile id="16" terrain="4,4,5,4"/>
<tile id="17" terrain="4,5,4,5"/>
<tile id="18" terrain="5,4,5,5"/>
<tile id="19" terrain="4,4,5,5"/>
<tile id="20" terrain="4,4,11,4"/>
<tile id="21" terrain="4,11,4,11"/>
<tile id="22" terrain="11,4,11,11"/>
<tile id="23" terrain="4,4,11,11"/>
<tile id="24" terrain="4,4,7,4"/>
<tile id="25" terrain="4,7,4,7"/>
<tile id="26" terrain="7,4,7,7"/>
<tile id="27" terrain="4,4,7,7"/>
<tile id="28" terrain="4,4,8,4"/>
<tile id="29" terrain="4,8,4,8"/>
<tile id="30" terrain="8,4,8,8"/>
<tile id="31" terrain="4,4,8,8"/>
<tile id="32" terrain="5,5,6,5"/>
<tile id="33" terrain="5,6,5,6"/>
<tile id="34" terrain="6,5,6,6"/>
<tile id="35" terrain="5,5,6,6"/>
<tile id="36" terrain="5,5,11,5"/>
<tile id="37" terrain="5,11,5,11"/>
<tile id="38" terrain="11,5,11,11"/>
<tile id="39" terrain="5,5,11,11"/>
<tile id="40" terrain="11,11,7,11"/>
<tile id="41" terrain="11,7,11,7"/>
<tile id="42" terrain="7,11,7,7"/>
<tile id="43" terrain="11,11,7,7"/>
<tile id="45" terrain="1,0,0,1"/>
<tile id="46" terrain="0,1,1,1"/>
<tile id="47" terrain="1,1,1,1"/>
<tile id="48" terrain="1,1,1,0"/>
<tile id="49" terrain="2,1,1,2"/>
<tile id="50" terrain="1,2,2,2"/>
<tile id="51" terrain="2,2,2,2"/>
<tile id="52" terrain="2,2,2,1"/>
<tile id="53" terrain="3,2,2,3"/>
<tile id="54" terrain="2,3,3,3"/>
<tile id="55" terrain="3,3,3,3"/>
<tile id="56" terrain="3,3,3,2"/>
<tile id="57" terrain="4,3,3,4">
	<properties>
		<property name="fence_count" type="int" value="2" />
		<property name="fence0" value="UR"/>
		<property name="fence1" value="DL"/>
	</properties>
</tile>
<tile id="58" terrain="3,4,4,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="59" terrain="4,4,4,4"/>
<tile id="60" terrain="4,4,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="61" terrain="5,4,4,5"/>
<tile id="62" terrain="4,5,5,5"/>
<tile id="63" terrain="5,5,5,5"/>
<tile id="64" terrain="5,5,5,4"/>
<tile id="65" terrain="11,4,4,11"/>
<tile id="66" terrain="4,11,11,11"/>
<tile id="67" terrain="11,11,11,11"/>
<tile id="68" terrain="11,11,11,4"/>
<tile id="69" terrain="7,4,4,7"/>
<tile id="70" terrain="4,7,7,7"/>
<tile id="71" terrain="7,7,7,7"/>
<tile id="72" terrain="7,7,7,4"/>
<tile id="73" terrain="8,4,4,8"/>
<tile id="74" terrain="4,8,8,8"/>
<tile id="75" terrain="8,8,8,8"/>
<tile id="76" terrain="8,8,8,4"/>
<tile id="77" terrain="6,5,5,6"/>
<tile id="78" terrain="5,6,6,6"/>
<tile id="79" terrain="6,6,6,6"/>
<tile id="80" terrain="6,6,6,5"/>
<tile id="81" terrain="11,5,5,11"/>
<tile id="82" terrain="5,11,11,11"/>
<tile id="83" terrain="11,11,11,11"/>
<tile id="84" terrain="11,11,11,5"/>
<tile id="85" terrain="7,11,11,7"/>
<tile id="86" terrain="11,7,7,7"/>
<tile id="87" terrain="7,7,7,7"/>
<tile id="88" terrain="7,7,7,11"/>
<tile id="90" terrain="0,1,0,0"/>
<tile id="91" terrain="1,1,0,0"/>
<tile id="92" terrain="1,1,0,1"/>
<tile id="93" terrain="1,0,1,0"/>
<tile id="94" terrain="1,2,1,1"/>
<tile id="95" terrain="2,2,1,1"/>
<tile id="96" terrain="2,2,1,2"/>
<tile id="97" terrain="2,1,2,1"/>
<tile id="98" terrain="2,3,2,2"/>
<tile id="99" terrain="3,3,2,2"/>
<tile id="100" terrain="3,3,2,3"/>
<tile id="101" terrain="3,2,3,2"/>
<tile id="102" terrain="3,4,3,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="103" terrain="4,4,3,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LR"/>
	</properties>
</tile>
<tile id="104" terrain="4,4,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="105" terrain="4,3,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UD"/>
	</properties>
</tile>
<tile id="106" terrain="4,5,4,4"/>
<tile id="107" terrain="5,5,4,4"/>
<tile id="108" terrain="5,5,4,5"/>
<tile id="109" terrain="5,4,5,4"/>
<tile id="110" terrain="4,11,4,4"/>
<tile id="111" terrain="11,11,4,4"/>
<tile id="112" terrain="11,11,4,11"/>
<tile id="113" terrain="11,4,11,4"/>
<tile id="114" terrain="4,7,4,4"/>
<tile id="115" terrain="7,7,4,4"/>
<tile id="116" terrain="7,7,4,7"/>
<tile id="117" terrain="7,4,7,4"/>
<tile id="118" terrain="4,8,4,4"/>
<tile id="119" terrain="8,8,4,4"/>
<tile id="120" terrain="8,8,4,8"/>
<tile id="121" terrain="8,4,8,4"/>
<tile id="122" terrain="5,6,5,5"/>
<tile id="123" terrain="6,6,5,5"/>
<tile id="124" terrain="6,6,5,6"/>
<tile id="125" terrain="6,5,6,5"/>
<tile id="126" terrain="5,11,5,5"/>
<tile id="127" terrain="11,11,5,5"/>
<tile id="128" terrain="11,11,5,11"/>
<tile id="129" terrain="11,5,11,5"/>
<tile id="130" terrain="11,7,11,11"/>
<tile id="131" terrain="7,7,11,11"/>
<tile id="132" terrain="7,7,11,7"/>
<tile id="133" terrain="7,11,7,11"/>
<tile id="135" terrain="0,0,0,0"/>
<tile id="136" terrain="0,0,0,1"/>
<tile id="137" terrain="0,1,1,0"/>
<tile id="138" terrain="1,0,0,0"/>
<tile id="139" terrain="1,1,1,1"/>
<tile id="140" terrain="1,1,1,2"/>
<tile id="141" terrain="1,2,2,1"/>
<tile id="142" terrain="2,1,1,1"/>
<tile id="143" terrain="2,2,2,2"/>
<tile id="144" terrain="2,2,2,3"/>
<tile id="145" terrain="2,3,3,2"/>
<tile id="146" terrain="3,2,2,2"/>
<tile id="147" terrain="3,3,3,3"/>
<tile id="148" terrain="3,3,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="149" terrain="3,4,4,3">
	<properties>
		<property name="fence_count" type="int" value="2" />
		<property name="fence0" value="UR"/>
		<property name="fence1" value="DL"/>
	</properties>
</tile>
<tile id="150" terrain="4,3,3,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="151" terrain="4,4,4,4"/>
<tile id="152" terrain="4,4,4,5"/>
<tile id="153" terrain="4,5,5,4"/>
<tile id="154" terrain="5,4,4,4"/>
<tile id="155" terrain="4,4,4,4"/>
<tile id="156" terrain="4,4,4,11"/>
<tile id="157" terrain="4,11,11,4"/>
<tile id="158" terrain="11,4,4,4"/>
<tile id="159" terrain="4,4,4,4"/>
<tile id="160" terrain="4,4,4,7"/>
<tile id="161" terrain="4,7,7,4"/>
<tile id="162" terrain="7,4,4,4"/>
<tile id="163" terrain="4,4,4,4"/>
<tile id="164" terrain="4,4,4,8"/>
<tile id="165" terrain="4,8,8,4"/>
<tile id="166" terrain="8,4,4,4"/>
<tile id="167" terrain="5,5,5,5"/>
<tile id="168" terrain="5,5,5,6"/>
<tile id="169" terrain="5,6,6,5"/>
<tile id="170" terrain="6,5,5,5"/>
<tile id="171" terrain="5,5,5,5"/>
<tile id="172" terrain="5,5,5,11"/>
<tile id="173" terrain="5,11,11,5"/>
<tile id="174" terrain="11,5,5,5"/>
<tile id="175" terrain="11,11,11,11"/>
<tile id="176" terrain="11,11,11,7"/>
<tile id="177" terrain="11,7,7,11"/>
<tile id="178" terrain="7,11,11,11"/>
<tile id="180" terrain="11,11,10,11"/>
<tile id="181" terrain="11,10,11,10"/>
<tile id="182" terrain="10,11,10,10"/>
<tile id="183" terrain="11,11,10,10"/>
<tile id="184" terrain="11,11,12,11"/>
<tile id="185" terrain="11,12,11,12"/>
<tile id="186" terrain="12,11,12,12"/>
<tile id="187" terrain="11,11,12,12"/>
<tile id="188" terrain="7,7,8,7"/>
<tile id="189" terrain="7,8,7,8"/>
<tile id="190" terrain="8,7,8,8"/>
<tile id="191" terrain="7,7,8,8"/>
<tile id="192" terrain="7,7,10,7"/>
<tile id="193" terrain="7,10,7,10"/>
<tile id="194" terrain="10,7,10,10"/>
<tile id="195" terrain="7,7,10,10"/>
<tile id="196" terrain="8,8,9,8"/>
<tile id="197" terrain="8,9,8,9"/>
<tile id="198" terrain="9,8,9,9"/>
<tile id="199" terrain="8,8,9,9"/>
<tile id="200" terrain="6,6,11,6"/>
<tile id="201" terrain="6,11,6,11"/>
<tile id="202" terrain="11,6,11,11"/>
<tile id="203" terrain="6,6,11,11"/>
<tile id="204" terrain="6,6,12,6"/>
<tile id="205" terrain="6,12,6,12"/>
<tile id="206" terrain="12,6,12,12"/>
<tile id="207" terrain="6,6,12,12"/>
<tile id="208" terrain="6,6,15,6"/>
<tile id="209" terrain="6,15,6,15"/>
<tile id="210" terrain="15,6,15,15"/>
<tile id="211" terrain="6,6,15,15"/>
<tile id="212" terrain="6,6,14,6"/>
<tile id="213" terrain="6,14,6,14"/>
<tile id="214" terrain="14,6,14,14"/>
<tile id="215" terrain="6,6,14,14"/>
<tile id="216" terrain="10,10,9,10"/>
<tile id="217" terrain="10,9,10,9"/>
<tile id="218" terrain="9,10,9,9"/>
<tile id="219" terrain="10,10,9,9"/>
<tile id="220" terrain="10,10,13,10"/>
<tile id="221" terrain="10,13,10,13"/>
<tile id="222" terrain="13,10,13,13"/>
<tile id="223" terrain="10,10,13,13"/>
<tile id="225" terrain="10,11,11,10"/>
<tile id="226" terrain="11,10,10,10"/>
<tile id="227" terrain="10,10,10,10"/>
<tile id="228" terrain="10,10,10,11"/>
<tile id="229" terrain="12,11,11,12"/>
<tile id="230" terrain="11,12,12,12"/>
<tile id="231" terrain="12,12,12,12"/>
<tile id="232" terrain="12,12,12,11"/>
<tile id="233" terrain="8,7,7,8"/>
<tile id="234" terrain="7,8,8,8"/>
<tile id="235" terrain="8,8,8,8"/>
<tile id="236" terrain="8,8,8,7"/>
<tile id="237" terrain="10,7,7,10"/>
<tile id="238" terrain="7,10,10,10"/>
<tile id="239" terrain="10,10,10,10"/>
<tile id="240" terrain="10,10,10,7"/>
<tile id="241" terrain="9,8,8,9"/>
<tile id="242" terrain="8,9,9,9"/>
<tile id="243" terrain="9,9,9,9"/>
<tile id="244" terrain="9,9,9,8"/>
<tile id="245" terrain="11,6,6,11"/>
<tile id="246" terrain="6,11,11,11"/>
<tile id="247" terrain="11,11,11,11"/>
<tile id="248" terrain="11,11,11,6"/>
<tile id="249" terrain="12,6,6,12"/>
<tile id="250" terrain="6,12,12,12"/>
<tile id="251" terrain="12,12,12,12"/>
<tile id="252" terrain="12,12,12,6"/>
<tile id="253" terrain="15,6,6,15"/>
<tile id="254" terrain="6,15,15,15"/>
<tile id="255" terrain="15,15,15,15"/>
<tile id="256" terrain="15,15,15,6"/>
<tile id="257" terrain="14,6,6,14"/>
<tile id="258" terrain="6,14,14,14"/>
<tile id="259" terrain="14,14,14,14"/>
<tile id="260" terrain="14,14,14,6"/>
<tile id="261" terrain="9,10,10,9"/>
<tile id="262" terrain="10,9,9,9"/>
<tile id="263" terrain="9,9,9,9"/>
<tile id="264" terrain="9,9,9,10"/>
<tile id="265" terrain="13,10,10,13"/>
<tile id="266" terrain="10,13,13,13"/>
<tile id="267" terrain="13,13,13,13"/>
<tile id="268" terrain="13,13,13,10"/>
<tile id="270" terrain="11,10,11,11"/>
<tile id="271" terrain="10,10,11,11"/>
<tile id="272" terrain="10,10,11,10"/>
<tile id="273" terrain="10,11,10,11"/>
<tile id="274" terrain="11,12,11,11"/>
<tile id="275" terrain="12,12,11,11"/>
<tile id="276" terrain="12,12,11,12"/>
<tile id="277" terrain="12,11,12,11"/>
<tile id="278" terrain="7,8,7,7"/>
<tile id="279" terrain="8,8,7,7"/>
<tile id="280" terrain="8,8,7,8"/>
<tile id="281" terrain="8,7,8,7"/>
<tile id="282" terrain="7,10,7,7"/>
<tile id="283" terrain="10,10,7,7"/>
<tile id="284" terrain="10,10,7,10"/>
<tile id="285" terrain="10,7,10,7"/>
<tile id="286" terrain="8,9,8,8"/>
<tile id="287" terrain="9,9,8,8"/>
<tile id="288" terrain="9,9,8,9"/>
<tile id="289" terrain="9,8,9,8"/>
<tile id="290" terrain="6,11,6,6"/>
<tile id="291" terrain="11,11,6,6"/>
<tile id="292" terrain="11,11,6,11"/>
<tile id="293" terrain="11,6,11,6"/>
<tile id="294" terrain="6,12,6,6"/>
<tile id="295" terrain="12,12,6,6"/>
<tile id="296" terrain="12,12,6,12"/>
<tile id="297" terrain="12,6,12,6"/>
<tile id="298" terrain="6,15,6,6"/>
<tile id="299" terrain="15,15,6,6"/>
<tile id="300" terrain="15,15,6,15"/>
<tile id="301" terrain="15,6,15,6"/>
<tile id="302" terrain="6,14,6,6"/>
<tile id="303" terrain="14,14,6,6"/>
<tile id="304" terrain="14,14,6,14"/>
<tile id="305" terrain="14,6,14,6"/>
<tile id="306" terrain="10,9,10,10"/>
<tile id="307" terrain="9,9,10,10"/>
<tile id="308" terrain="9,9,10,9"/>
<tile id="309" terrain="9,10,9,10"/>
<tile id="310" terrain="10,13,10,10"/>
<tile id="311" terrain="13,13,10,10"/>
<tile id="312" terrain="13,13,10,13"/>
<tile id="313" terrain="13,10,13,10"/>
<tile id="315" terrain="11,11,11,11"/>
<tile id="316" terrain="11,11,11,10"/>
<tile id="317" terrain="11,10,10,11"/>
<tile id="318" terrain="10,11,11,11"/>
<tile id="319" terrain="11,11,11,11"/>
<tile id="320" terrain="11,11,11,12"/>
<tile id="321" terrain="11,12,12,11"/>
<tile id="322" terrain="12,11,11,11"/>
<tile id="323" terrain="7,7,7,7"/>
<tile id="324" terrain="7,7,7,8"/>
<tile id="325" terrain="7,8,8,7"/>
<tile id="326" terrain="8,7,7,7"/>
<tile id="327" terrain="7,7,7,7"/>
<tile id="328" terrain="7,7,7,10"/>
<tile id="329" terrain="7,10,10,7"/>
<tile id="330" terrain="10,7,7,7"/>
<tile id="331" terrain="8,8,8,8"/>
<tile id="332" terrain="8,8,8,9"/>
<tile id="333" terrain="8,9,9,8"/>
<tile id="334" terrain="9,8,8,8"/>
<tile id="335" terrain="6,6,6,6"/>
<tile id="336" terrain="6,6,6,11"/>
<tile id="337" terrain="6,11,11,6"/>
<tile id="338" terrain="11,6,6,6"/>
<tile id="339" terrain="6,6,6,6"/>
<tile id="340" terrain="6,6,6,12"/>
<tile id="341" terrain="6,12,12,6"/>
<tile id="342" terrain="12,6,6,6"/>
<tile id="343" terrain="6,6,6,6"/>
<tile id="344" terrain="6,6,6,15"/>
<tile id="345" terrain="6,15,15,6"/>
<tile id="346" terrain="15,6,6,6"/>
<tile id="347" terrain="6,6,6,6"/>
<tile id="348" terrain="6,6,6,14"/>
<tile id="349" terrain="6,14,14,6"/>
<tile id="350" terrain="14,6,6,6"/>
<tile id="351" terrain="10,10,10,10"/>
<tile id="352" terrain="10,10,10,9"/>
<tile id="353" terrain="10,9,9,10"/>
<tile id="354" terrain="9,10,10,10"/>
<tile id="355" terrain="10,10,10,10"/>
<tile id="356" terrain="10,10,10,13"/>
<tile id="357" terrain="10,13,13,10"/>
<tile id="358" terrain="13,10,10,10"/>
<tile id="360" terrain="10,10,12,10"/>
<tile id="361" terrain="10,12,10,12"/>
<tile id="362" terrain="12,10,12,12"/>
<tile id="363" terrain="10,10,12,12"/>
<tile id="364" terrain="9,9,13,9"/>
<tile id="365" terrain="9,13,9,13"/>
<tile id="366" terrain="13,9,13,13"/>
<tile id="367" terrain="9,9,13,13"/>
<tile id="368" terrain="12,12,13,12"/>
<tile id="369" terrain="12,13,12,13"/>
<tile id="370" terrain="13,12,13,13"/>
<tile id="371" terrain="12,12,13,13"/>
<tile id="372" terrain="12,12,17,12"/>
<tile id="373" terrain="12,17,12,17"/>
<tile id="374" terrain="17,12,17,17"/>
<tile id="375" terrain="12,12,17,17"/>
<tile id="376" terrain="12,12,16,12"/>
<tile id="377" terrain="12,16,12,16"/>
<tile id="378" terrain="16,12,16,16"/>
<tile id="379" terrain="12,12,16,16"/>
<tile id="380" terrain="13,13,17,13"/>
<tile id="381" terrain="13,17,13,17"/>
<tile id="382" terrain="17,13,17,17"/>
<tile id="383" terrain="13,13,17,17"/>
<tile id="384" terrain="14,14,15,14"/>
<tile id="385" terrain="14,15,14,15"/>
<tile id="386" terrain="15,14,15,15"/>
<tile id="387" terrain="14,14,15,15"/>
<tile id="388" terrain="15,15,16,15"/>
<tile id="389" terrain="15,16,15,16"/>
<tile id="390" terrain="16,15,16,16"/>
<tile id="391" terrain="15,15,16,16"/>
<tile id="392" terrain="16,16,17,16"/>
<tile id="393" terrain="16,17,16,17"/>
<tile id="394" terrain="17,16,17,17"/>
<tile id="395" terrain="16,16,17,17"/>
<tile id="396" terrain="12,12,15,12"/>
<tile id="397" terrain="12,15,12,15"/>
<tile id="398" terrain="15,12,15,15"/>
<tile id="399" terrain="12,12,15,15"/>
<tile id="400" terrain="21,21,3,21"/>
<tile id="401" terrain="21,3,21,3"/>
<tile id="402" terrain="3,21,3,3"/>
<tile id="403" terrain="21,21,3,3"/>
<tile id="405" terrain="12,10,10,12"/>
<tile id="406" terrain="10,12,12,12"/>
<tile id="407" terrain="12,12,12,12"/>
<tile id="408" terrain="12,12,12,10"/>
<tile id="409" terrain="13,9,9,13"/>
<tile id="410" terrain="9,13,13,13"/>
<tile id="411" terrain="13,13,13,13"/>
<tile id="412" terrain="13,13,13,9"/>
<tile id="413" terrain="13,12,12,13"/>
<tile id="414" terrain="12,13,13,13"/>
<tile id="415" terrain="13,13,13,13"/>
<tile id="416" terrain="13,13,13,12"/>
<tile id="417" terrain="17,12,12,17"/>
<tile id="418" terrain="12,17,17,17"/>
<tile id="419" terrain="17,17,17,17"/>
<tile id="420" terrain="17,17,17,12"/>
<tile id="421" terrain="16,12,12,16"/>
<tile id="422" terrain="12,16,16,16"/>
<tile id="423" terrain="16,16,16,16"/>
<tile id="424" terrain="16,16,16,12"/>
<tile id="425" terrain="17,13,13,17"/>
<tile id="426" terrain="13,17,17,17"/>
<tile id="427" terrain="17,17,17,17"/>
<tile id="428" terrain="17,17,17,13"/>
<tile id="429" terrain="15,14,14,15"/>
<tile id="430" terrain="14,15,15,15"/>
<tile id="431" terrain="15,15,15,15"/>
<tile id="432" terrain="15,15,15,14"/>
<tile id="433" terrain="16,15,15,16"/>
<tile id="434" terrain="15,16,16,16"/>
<tile id="435" terrain="16,16,16,16"/>
<tile id="436" terrain="16,16,16,15"/>
<tile id="437" terrain="17,16,16,17"/>
<tile id="438" terrain="16,17,17,17"/>
<tile id="439" terrain="17,17,17,17"/>
<tile id="440" terrain="17,17,17,16"/>
<tile id="441" terrain="15,12,12,15"/>
<tile id="442" terrain="12,15,15,15"/>
<tile id="443" terrain="15,15,15,15"/>
<tile id="444" terrain="15,15,15,12"/>
<tile id="445" terrain="3,21,21,3"/>
<tile id="446" terrain="21,3,3,3"/>
<tile id="447" terrain="3,3,3,3"/>
<tile id="448" terrain="3,3,3,21"/>
<tile id="450" terrain="10,12,10,10"/>
<tile id="451" terrain="12,12,10,10"/>
<tile id="452" terrain="12,12,10,12"/>
<tile id="453" terrain="12,10,12,10"/>
<tile id="454" terrain="9,13,9,9"/>
<tile id="455" terrain="13,13,9,9"/>
<tile id="456" terrain="13,13,9,13"/>
<tile id="457" terrain="13,9,13,9"/>
<tile id="458" terrain="12,13,12,12"/>
<tile id="459" terrain="13,13,12,12"/>
<tile id="460" terrain="13,13,12,13"/>
<tile id="461" terrain="13,12,13,12"/>
<tile id="462" terrain="12,17,12,12"/>
<tile id="463" terrain="17,17,12,12"/>
<tile id="464" terrain="17,17,12,17"/>
<tile id="465" terrain="17,12,17,12"/>
<tile id="466" terrain="12,16,12,12"/>
<tile id="467" terrain="16,16,12,12"/>
<tile id="468" terrain="16,16,12,16"/>
<tile id="469" terrain="16,12,16,12"/>
<tile id="470" terrain="13,17,13,13"/>
<tile id="471" terrain="17,17,13,13"/>
<tile id="472" terrain="17,17,13,17"/>
<tile id="473" terrain="17,13,17,13"/>
<tile id="474" terrain="14,15,14,14"/>
<tile id="475" terrain="15,15,14,14"/>
<tile id="476" terrain="15,15,14,15"/>
<tile id="477" terrain="15,14,15,14"/>
<tile id="478" terrain="15,16,15,15"/>
<tile id="479" terrain="16,16,15,15"/>
<tile id="480" terrain="16,16,15,16"/>
<tile id="481" terrain="16,15,16,15"/>
<tile id="482" terrain="16,17,16,16"/>
<tile id="483" terrain="17,17,16,16"/>
<tile id="484" terrain="17,17,16,17"/>
<tile id="485" terrain="17,16,17,16"/>
<tile id="486" terrain="12,15,12,12"/>
<tile id="487" terrain="15,15,12,12"/>
<tile id="488" terrain="15,15,12,15"/>
<tile id="489" terrain="15,12,15,12"/>
<tile id="490" terrain="21,3,21,21"/>
<tile id="491" terrain="3,3,21,21"/>
<tile id="492" terrain="3,3,21,3"/>
<tile id="493" terrain="3,21,3,21"/>
<tile id="495" terrain="10,10,10,10"/>
<tile id="496" terrain="10,10,10,12"/>
<tile id="497" terrain="10,12,12,10"/>
<tile id="498" terrain="12,10,10,10"/>
<tile id="499" terrain="9,9,9,9"/>
<tile id="500" terrain="9,9,9,13"/>
<tile id="501" terrain="9,13,13,9"/>
<tile id="502" terrain="13,9,9,9"/>
<tile id="503" terrain="12,12,12,12"/>
<tile id="504" terrain="12,12,12,13"/>
<tile id="505" terrain="12,13,13,12"/>
<tile id="506" terrain="13,12,12,12"/>
<tile id="507" terrain="12,12,12,12"/>
<tile id="508" terrain="12,12,12,17"/>
<tile id="509" terrain="12,17,17,12"/>
<tile id="510" terrain="17,12,12,12"/>
<tile id="511" terrain="12,12,12,12"/>
<tile id="512" terrain="12,12,12,16"/>
<tile id="513" terrain="12,16,16,12"/>
<tile id="514" terrain="16,12,12,12"/>
<tile id="515" terrain="13,13,13,13"/>
<tile id="516" terrain="13,13,13,17"/>
<tile id="517" terrain="13,17,17,13"/>
<tile id="518" terrain="17,13,13,13"/>
<tile id="519" terrain="14,14,14,14"/>
<tile id="520" terrain="14,14,14,15"/>
<tile id="521" terrain="14,15,15,14"/>
<tile id="522" terrain="15,14,14,14"/>
<tile id="523" terrain="15,15,15,15"/>
<tile id="524" terrain="15,15,15,16"/>
<tile id="525" terrain="15,16,16,15"/>
<tile id="526" terrain="16,15,15,15"/>
<tile id="527" terrain="16,16,16,16"/>
<tile id="528" terrain="16,16,16,17"/>
<tile id="529" terrain="16,17,17,16"/>
<tile id="530" terrain="17,16,16,16"/>
<tile id="531" terrain="12,12,12,12"/>
<tile id="532" terrain="12,12,12,15"/>
<tile id="533" terrain="12,15,15,12"/>
<tile id="534" terrain="15,12,12,12"/>
<tile id="535" terrain="21,21,21,21"/>
<tile id="536" terrain="21,21,21,3"/>
<tile id="537" terrain="21,3,3,21"/>
<tile id="538" terrain="3,21,21,21"/>
<tile id="540" terrain="4,4,5,11"/>
<tile id="541" terrain="5,5,11,4"/>
<tile id="542" terrain="11,11,4,5"/>
<tile id="543" terrain="4,4,11,5"/>
<tile id="544" terrain="11,11,5,4"/>
<tile id="545" terrain="5,5,4,11"/>
<tile id="546" terrain="4,5,11,4"/>
<tile id="547" terrain="5,11,4,5"/>
<tile id="548" terrain="11,4,5,11"/>
<tile id="549" terrain="4,4,11,7"/>
<tile id="550" terrain="11,11,7,4"/>
<tile id="551" terrain="7,7,4,11"/>
<tile id="552" terrain="4,4,7,11"/>
<tile id="553" terrain="7,7,11,4"/>
<tile id="554" terrain="11,11,4,7"/>
<tile id="555" terrain="4,11,7,4"/>
<tile id="556" terrain="11,7,4,11"/>
<tile id="557" terrain="7,4,11,7"/>
<tile id="558" terrain="4,4,7,8"/>
<tile id="559" terrain="7,7,8,4"/>
<tile id="560" terrain="8,8,4,7"/>
<tile id="561" terrain="4,4,8,7"/>
<tile id="562" terrain="8,8,7,4"/>
<tile id="563" terrain="7,7,4,8"/>
<tile id="564" terrain="4,7,8,4"/>
<tile id="565" terrain="7,8,4,7"/>
<tile id="566" terrain="8,4,7,8"/>
<tile id="567" terrain="8,8,9,10"/>
<tile id="568" terrain="9,9,10,8"/>
<tile id="569" terrain="10,10,8,9"/>
<tile id="570" terrain="8,8,10,9"/>
<tile id="571" terrain="10,10,9,8"/>
<tile id="572" terrain="9,9,8,10"/>
<tile id="573" terrain="8,9,10,8"/>
<tile id="574" terrain="9,10,8,9"/>
<tile id="575" terrain="10,8,9,10"/>
<tile id="576" terrain="8,8,7,10"/>
<tile id="577" terrain="7,7,10,8"/>
<tile id="578" terrain="10,10,8,7"/>
<tile id="579" terrain="8,8,10,7"/>
<tile id="580" terrain="10,10,7,8"/>
<tile id="581" terrain="7,7,8,10"/>
<tile id="582" terrain="8,7,10,8"/>
<tile id="583" terrain="7,10,8,7"/>
<tile id="584" terrain="10,8,7,10"/>
<tile id="585" terrain="4,11,4,5"/>
<tile id="586" terrain="5,4,5,11"/>
<tile id="587" terrain="11,5,11,4"/>
<tile id="588" terrain="4,5,4,11"/>
<tile id="589" terrain="11,4,11,5"/>
<tile id="590" terrain="5,11,5,4"/>
<tile id="591" terrain="5,4,4,11"/>
<tile id="592" terrain="11,5,5,4"/>
<tile id="593" terrain="4,11,11,5"/>
<tile id="594" terrain="4,7,4,11"/>
<tile id="595" terrain="11,4,11,7"/>
<tile id="596" terrain="7,11,7,4"/>
<tile id="597" terrain="4,11,4,7"/>
<tile id="598" terrain="7,4,7,11"/>
<tile id="599" terrain="11,7,11,4"/>
<tile id="600" terrain="11,4,4,7"/>
<tile id="601" terrain="7,11,11,4"/>
<tile id="602" terrain="4,7,7,11"/>
<tile id="603" terrain="4,8,4,7"/>
<tile id="604" terrain="7,4,7,8"/>
<tile id="605" terrain="8,7,8,4"/>
<tile id="606" terrain="4,7,4,8"/>
<tile id="607" terrain="8,4,8,7"/>
<tile id="608" terrain="7,8,7,4"/>
<tile id="609" terrain="7,4,4,8"/>
<tile id="610" terrain="8,7,7,4"/>
<tile id="611" terrain="4,8,8,7"/>
<tile id="612" terrain="8,10,8,9"/>
<tile id="613" terrain="9,8,9,10"/>
<tile id="614" terrain="10,9,10,8"/>
<tile id="615" terrain="8,9,8,10"/>
<tile id="616" terrain="10,8,10,9"/>
<tile id="617" terrain="9,10,9,8"/>
<tile id="618" terrain="9,8,8,10"/>
<tile id="619" terrain="10,9,9,8"/>
<tile id="620" terrain="8,10,10,9"/>
<tile id="621" terrain="8,10,8,7"/>
<tile id="622" terrain="7,8,7,10"/>
<tile id="623" terrain="10,7,10,8"/>
<tile id="624" terrain="8,7,8,10"/>
<tile id="625" terrain="10,8,10,7"/>
<tile id="626" terrain="7,10,7,8"/>
<tile id="627" terrain="7,8,8,10"/>
<tile id="628" terrain="10,7,7,8"/>
<tile id="629" terrain="8,10,10,7"/>
<tile id="630" terrain="11,5,4,4"/>
<tile id="631" terrain="4,11,5,5"/>
<tile id="632" terrain="5,4,11,11"/>
<tile id="633" terrain="5,11,4,4"/>
<tile id="634" terrain="4,5,11,11"/>
<tile id="635" terrain="11,4,5,5"/>
<tile id="636" terrain="4,11,5,4"/>
<tile id="637" terrain="5,4,11,5"/>
<tile id="638" terrain="11,5,4,11"/>
<tile id="639" terrain="7,11,4,4"/>
<tile id="640" terrain="4,7,11,11"/>
<tile id="641" terrain="11,4,7,7"/>
<tile id="642" terrain="11,7,4,4"/>
<tile id="643" terrain="4,11,7,7"/>
<tile id="644" terrain="7,4,11,11"/>
<tile id="645" terrain="4,7,11,4"/>
<tile id="646" terrain="11,4,7,11"/>
<tile id="647" terrain="7,11,4,7"/>
<tile id="648" terrain="8,7,4,4"/>
<tile id="649" terrain="4,8,7,7"/>
<tile id="650" terrain="7,4,8,8"/>
<tile id="651" terrain="7,8,4,4"/>
<tile id="652" terrain="4,7,8,8"/>
<tile id="653" terrain="8,4,7,7"/>
<tile id="654" terrain="4,8,7,4"/>
<tile id="655" terrain="7,4,8,7"/>
<tile id="656" terrain="8,7,4,8"/>
<tile id="657" terrain="10,9,8,8"/>
<tile id="658" terrain="8,10,9,9"/>
<tile id="659" terrain="9,8,10,10"/>
<tile id="660" terrain="9,10,8,8"/>
<tile id="661" terrain="8,9,10,10"/>
<tile id="662" terrain="10,8,9,9"/>
<tile id="663" terrain="8,10,9,8"/>
<tile id="664" terrain="9,8,10,9"/>
<tile id="665" terrain="10,9,8,10"/>
<tile id="666" terrain="10,7,8,8"/>
<tile id="667" terrain="8,10,7,7"/>
<tile id="668" terrain="7,8,10,10"/>
<tile id="669" terrain="7,10,8,8"/>
<tile id="670" terrain="8,7,10,10"/>
<tile id="671" terrain="10,8,7,7"/>
<tile id="672" terrain="8,10,7,8"/>
<tile id="673" terrain="7,8,10,7"/>
<tile id="674" terrain="10,7,8,10"/>
<tile id="675" terrain="5,4,11,4"/>
<tile id="676" terrain="11,5,4,5"/>
<tile id="677" terrain="4,11,5,11"/>
<tile id="678" terrain="11,4,5,4"/>
<tile id="679" terrain="5,11,4,11"/>
<tile id="680" terrain="4,5,11,5"/>
<tile id="681" terrain="11,4,4,5"/>
<tile id="682" terrain="4,5,5,11"/>
<tile id="683" terrain="5,11,11,4"/>
<tile id="684" terrain="11,4,7,4"/>
<tile id="685" terrain="7,11,4,11"/>
<tile id="686" terrain="4,7,11,7"/>
<tile id="687" terrain="7,4,11,4"/>
<tile id="688" terrain="11,7,4,7"/>
<tile id="689" terrain="4,11,7,11"/>
<tile id="690" terrain="7,4,4,11"/>
<tile id="691" terrain="4,11,11,7"/>
<tile id="692" terrain="11,7,7,4"/>
<tile id="693" terrain="7,4,8,4"/>
<tile id="694" terrain="8,7,4,7"/>
<tile id="695" terrain="4,8,7,8"/>
<tile id="696" terrain="8,4,7,4"/>
<tile id="697" terrain="7,8,4,8"/>
<tile id="698" terrain="4,7,8,7"/>
<tile id="699" terrain="8,4,4,7"/>
<tile id="700" terrain="4,7,7,8"/>
<tile id="701" terrain="7,8,8,4"/>
<tile id="702" terrain="9,8,10,8"/>
<tile id="703" terrain="10,9,8,9"/>
<tile id="704" terrain="8,10,9,10"/>
<tile id="705" terrain="10,8,9,8"/>
<tile id="706" terrain="9,10,8,10"/>
<tile id="707" terrain="8,9,10,9"/>
<tile id="708" terrain="10,8,8,9"/>
<tile id="709" terrain="8,9,9,10"/>
<tile id="710" terrain="9,10,10,8"/>
<tile id="711" terrain="7,8,10,8"/>
<tile id="712" terrain="10,7,8,7"/>
<tile id="713" terrain="8,10,7,10"/>
<tile id="714" terrain="10,8,7,8"/>
<tile id="715" terrain="7,10,8,10"/>
<tile id="716" terrain="8,7,10,7"/>
<tile id="717" terrain="10,8,8,7"/>
<tile id="718" terrain="8,7,7,10"/>
<tile id="719" terrain="7,10,10,8"/>
<tile id="720" terrain="7,7,10,11"/>
<tile id="721" terrain="10,10,11,7"/>
<tile id="722" terrain="11,11,7,10"/>
<tile id="723" terrain="7,7,11,10"/>
<tile id="724" terrain="11,11,10,7"/>
<tile id="725" terrain="10,10,7,11"/>
<tile id="726" terrain="7,10,11,7"/>
<tile id="727" terrain="10,11,7,10"/>
<tile id="728" terrain="11,7,10,11"/>
<tile id="729" terrain="11,11,5,6"/>
<tile id="730" terrain="5,5,6,11"/>
<tile id="731" terrain="6,6,11,5"/>
<tile id="732" terrain="11,11,6,5"/>
<tile id="733" terrain="6,6,5,11"/>
<tile id="734" terrain="5,5,11,6"/>
<tile id="735" terrain="11,5,6,11"/>
<tile id="736" terrain="5,6,11,5"/>
<tile id="737" terrain="6,11,5,6"/>
<tile id="738" terrain="9,9,10,13"/>
<tile id="739" terrain="10,10,13,9"/>
<tile id="740" terrain="13,13,9,10"/>
<tile id="741" terrain="9,9,13,10"/>
<tile id="742" terrain="13,13,10,9"/>
<tile id="743" terrain="10,10,9,13"/>
<tile id="744" terrain="9,10,13,9"/>
<tile id="745" terrain="10,13,9,10"/>
<tile id="746" terrain="13,9,10,13"/>
<tile id="747" terrain="10,10,13,12"/>
<tile id="748" terrain="13,13,12,10"/>
<tile id="749" terrain="12,12,10,13"/>
<tile id="750" terrain="10,10,12,13"/>
<tile id="751" terrain="12,12,13,10"/>
<tile id="752" terrain="13,13,10,12"/>
<tile id="753" terrain="10,13,12,10"/>
<tile id="754" terrain="13,12,10,13"/>
<tile id="755" terrain="12,10,13,12"/>
<tile id="756" terrain="10,10,11,12"/>
<tile id="757" terrain="11,11,12,10"/>
<tile id="758" terrain="12,12,10,11"/>
<tile id="759" terrain="10,10,12,11"/>
<tile id="760" terrain="12,12,11,10"/>
<tile id="761" terrain="11,11,10,12"/>
<tile id="762" terrain="10,11,12,10"/>
<tile id="763" terrain="11,12,10,11"/>
<tile id="764" terrain="12,10,11,12"/>
<tile id="765" terrain="7,11,7,10"/>
<tile id="766" terrain="10,7,10,11"/>
<tile id="767" terrain="11,10,11,7"/>
<tile id="768" terrain="7,10,7,11"/>
<tile id="769" terrain="11,7,11,10"/>
<tile id="770" terrain="10,11,10,7"/>
<tile id="771" terrain="10,7,7,11"/>
<tile id="772" terrain="11,10,10,7"/>
<tile id="773" terrain="7,11,11,10"/>
<tile id="774" terrain="11,6,11,5"/>
<tile id="775" terrain="5,11,5,6"/>
<tile id="776" terrain="6,5,6,11"/>
<tile id="777" terrain="11,5,11,6"/>
<tile id="778" terrain="6,11,6,5"/>
<tile id="779" terrain="5,6,5,11"/>
<tile id="780" terrain="5,11,11,6"/>
<tile id="781" terrain="6,5,5,11"/>
<tile id="782" terrain="11,6,6,5"/>
<tile id="783" terrain="9,13,9,10"/>
<tile id="784" terrain="10,9,10,13"/>
<tile id="785" terrain="13,10,13,9"/>
<tile id="786" terrain="9,10,9,13"/>
<tile id="787" terrain="13,9,13,10"/>
<tile id="788" terrain="10,13,10,9"/>
<tile id="789" terrain="10,9,9,13"/>
<tile id="790" terrain="13,10,10,9"/>
<tile id="791" terrain="9,13,13,10"/>
<tile id="792" terrain="10,12,10,13"/>
<tile id="793" terrain="13,10,13,12"/>
<tile id="794" terrain="12,13,12,10"/>
<tile id="795" terrain="10,13,10,12"/>
<tile id="796" terrain="12,10,12,13"/>
<tile id="797" terrain="13,12,13,10"/>
<tile id="798" terrain="13,10,10,12"/>
<tile id="799" terrain="12,13,13,10"/>
<tile id="800" terrain="10,12,12,13"/>
<tile id="801" terrain="10,12,10,11"/>
<tile id="802" terrain="11,10,11,12"/>
<tile id="803" terrain="12,11,12,10"/>
<tile id="804" terrain="10,11,10,12"/>
<tile id="805" terrain="12,10,12,11"/>
<tile id="806" terrain="11,12,11,10"/>
<tile id="807" terrain="11,10,10,12"/>
<tile id="808" terrain="12,11,11,10"/>
<tile id="809" terrain="10,12,12,11"/>
<tile id="810" terrain="11,10,7,7"/>
<tile id="811" terrain="7,11,10,10"/>
<tile id="812" terrain="10,7,11,11"/>
<tile id="813" terrain="10,11,7,7"/>
<tile id="814" terrain="7,10,11,11"/>
<tile id="815" terrain="11,7,10,10"/>
<tile id="816" terrain="7,11,10,7"/>
<tile id="817" terrain="10,7,11,10"/>
<tile id="818" terrain="11,10,7,11"/>
<tile id="819" terrain="6,5,11,11"/>
<tile id="820" terrain="11,6,5,5"/>
<tile id="821" terrain="5,11,6,6"/>
<tile id="822" terrain="5,6,11,11"/>
<tile id="823" terrain="11,5,6,6"/>
<tile id="824" terrain="6,11,5,5"/>
<tile id="825" terrain="11,6,5,11"/>
<tile id="826" terrain="5,11,6,5"/>
<tile id="827" terrain="6,5,11,6"/>
<tile id="828" terrain="13,10,9,9"/>
<tile id="829" terrain="9,13,10,10"/>
<tile id="830" terrain="10,9,13,13"/>
<tile id="831" terrain="10,13,9,9"/>
<tile id="832" terrain="9,10,13,13"/>
<tile id="833" terrain="13,9,10,10"/>
<tile id="834" terrain="9,13,10,9"/>
<tile id="835" terrain="10,9,13,10"/>
<tile id="836" terrain="13,10,9,13"/>
<tile id="837" terrain="12,13,10,10"/>
<tile id="838" terrain="10,12,13,13"/>
<tile id="839" terrain="13,10,12,12"/>
<tile id="840" terrain="13,12,10,10"/>
<tile id="841" terrain="10,13,12,12"/>
<tile id="842" terrain="12,10,13,13"/>
<tile id="843" terrain="10,12,13,10"/>
<tile id="844" terrain="13,10,12,13"/>
<tile id="845" terrain="12,13,10,12"/>
<tile id="846" terrain="12,11,10,10"/>
<tile id="847" terrain="10,12,11,11"/>
<tile id="848" terrain="11,10,12,12"/>
<tile id="849" terrain="11,12,10,10"/>
<tile id="850" terrain="10,11,12,12"/>
<tile id="851" terrain="12,10,11,11"/>
<tile id="852" terrain="10,12,11,10"/>
<tile id="853" terrain="11,10,12,11"/>
<tile id="854" terrain="12,11,10,12"/>
<tile id="855" terrain="10,7,11,7"/>
<tile id="856" terrain="11,10,7,10"/>
<tile id="857" terrain="7,11,10,11"/>
<tile id="858" terrain="11,7,10,7"/>
<tile id="859" terrain="10,11,7,11"/>
<tile id="860" terrain="7,10,11,10"/>
<tile id="861" terrain="11,7,7,10"/>
<tile id="862" terrain="7,10,10,11"/>
<tile id="863" terrain="10,11,11,7"/>
<tile id="864" terrain="5,11,6,11"/>
<tile id="865" terrain="6,5,11,5"/>
<tile id="866" terrain="11,6,5,6"/>
<tile id="867" terrain="6,11,5,11"/>
<tile id="868" terrain="5,6,11,6"/>
<tile id="869" terrain="11,5,6,5"/>
<tile id="870" terrain="6,11,11,5"/>
<tile id="871" terrain="11,5,5,6"/>
<tile id="872" terrain="5,6,6,11"/>
<tile id="873" terrain="10,9,13,9"/>
<tile id="874" terrain="13,10,9,10"/>
<tile id="875" terrain="9,13,10,13"/>
<tile id="876" terrain="13,9,10,9"/>
<tile id="877" terrain="10,13,9,13"/>
<tile id="878" terrain="9,10,13,10"/>
<tile id="879" terrain="13,9,9,10"/>
<tile id="880" terrain="9,10,10,13"/>
<tile id="881" terrain="10,13,13,9"/>
<tile id="882" terrain="13,10,12,10"/>
<tile id="883" terrain="12,13,10,13"/>
<tile id="884" terrain="10,12,13,12"/>
<tile id="885" terrain="12,10,13,10"/>
<tile id="886" terrain="13,12,10,12"/>
<tile id="887" terrain="10,13,12,13"/>
<tile id="888" terrain="12,10,10,13"/>
<tile id="889" terrain="10,13,13,12"/>
<tile id="890" terrain="13,12,12,10"/>
<tile id="891" terrain="11,10,12,10"/>
<tile id="892" terrain="12,11,10,11"/>
<tile id="893" terrain="10,12,11,12"/>
<tile id="894" terrain="12,10,11,10"/>
<tile id="895" terrain="11,12,10,12"/>
<tile id="896" terrain="10,11,12,11"/>
<tile id="897" terrain="12,10,10,11"/>
<tile id="898" terrain="10,11,11,12"/>
<tile id="899" terrain="11,12,12,10"/>
<tile id="900" terrain="11,11,12,6"/>
<tile id="901" terrain="12,12,6,11"/>
<tile id="902" terrain="6,6,11,12"/>
<tile id="903" terrain="11,11,6,12"/>
<tile id="904" terrain="6,6,12,11"/>
<tile id="905" terrain="12,12,11,6"/>
<tile id="906" terrain="11,12,6,11"/>
<tile id="907" terrain="12,6,11,12"/>
<tile id="908" terrain="6,11,12,6"/>
<tile id="909" terrain="13,13,12,17"/>
<tile id="910" terrain="12,12,17,13"/>
<tile id="911" terrain="17,17,13,12"/>
<tile id="912" terrain="13,13,17,12"/>
<tile id="913" terrain="17,17,12,13"/>
<tile id="914" terrain="12,12,13,17"/>
<tile id="915" terrain="13,12,17,13"/>
<tile id="916" terrain="12,17,13,12"/>
<tile id="917" terrain="17,13,12,17"/>
<tile id="918" terrain="12,12,17,16"/>
<tile id="919" terrain="17,17,16,12"/>
<tile id="920" terrain="16,16,12,17"/>
<tile id="921" terrain="12,12,16,17"/>
<tile id="922" terrain="16,16,17,12"/>
<tile id="923" terrain="17,17,12,16"/>
<tile id="924" terrain="12,17,16,12"/>
<tile id="925" terrain="17,16,12,17"/>
<tile id="926" terrain="16,12,17,16"/>
<tile id="927" terrain="12,12,16,15"/>
<tile id="928" terrain="16,16,15,12"/>
<tile id="929" terrain="15,15,12,16"/>
<tile id="930" terrain="12,12,15,16"/>
<tile id="931" terrain="15,15,16,12"/>
<tile id="932" terrain="16,16,12,15"/>
<tile id="933" terrain="12,16,15,12"/>
<tile id="934" terrain="16,15,12,16"/>
<tile id="935" terrain="15,12,16,15"/>
<tile id="936" terrain="12,12,6,15"/>
<tile id="937" terrain="6,6,15,12"/>
<tile id="938" terrain="15,15,12,6"/>
<tile id="939" terrain="12,12,15,6"/>
<tile id="940" terrain="15,15,6,12"/>
<tile id="941" terrain="6,6,12,15"/>
<tile id="942" terrain="12,6,15,12"/>
<tile id="943" terrain="6,15,12,6"/>
<tile id="944" terrain="15,12,6,15"/>
<tile id="945" terrain="11,6,11,12"/>
<tile id="946" terrain="12,11,12,6"/>
<tile id="947" terrain="6,12,6,11"/>
<tile id="948" terrain="11,12,11,6"/>
<tile id="949" terrain="6,11,6,12"/>
<tile id="950" terrain="12,6,12,11"/>
<tile id="951" terrain="12,11,11,6"/>
<tile id="952" terrain="6,12,12,11"/>
<tile id="953" terrain="11,6,6,12"/>
<tile id="954" terrain="13,17,13,12"/>
<tile id="955" terrain="12,13,12,17"/>
<tile id="956" terrain="17,12,17,13"/>
<tile id="957" terrain="13,12,13,17"/>
<tile id="958" terrain="17,13,17,12"/>
<tile id="959" terrain="12,17,12,13"/>
<tile id="960" terrain="12,13,13,17"/>
<tile id="961" terrain="17,12,12,13"/>
<tile id="962" terrain="13,17,17,12"/>
<tile id="963" terrain="12,16,12,17"/>
<tile id="964" terrain="17,12,17,16"/>
<tile id="965" terrain="16,17,16,12"/>
<tile id="966" terrain="12,17,12,16"/>
<tile id="967" terrain="16,12,16,17"/>
<tile id="968" terrain="17,16,17,12"/>
<tile id="969" terrain="17,12,12,16"/>
<tile id="970" terrain="16,17,17,12"/>
<tile id="971" terrain="12,16,16,17"/>
<tile id="972" terrain="12,15,12,16"/>
<tile id="973" terrain="16,12,16,15"/>
<tile id="974" terrain="15,16,15,12"/>
<tile id="975" terrain="12,16,12,15"/>
<tile id="976" terrain="15,12,15,16"/>
<tile id="977" terrain="16,15,16,12"/>
<tile id="978" terrain="16,12,12,15"/>
<tile id="979" terrain="15,16,16,12"/>
<tile id="980" terrain="12,15,15,16"/>
<tile id="981" terrain="12,15,12,6"/>
<tile id="982" terrain="6,12,6,15"/>
<tile id="983" terrain="15,6,15,12"/>
<tile id="984" terrain="12,6,12,15"/>
<tile id="985" terrain="15,12,15,6"/>
<tile id="986" terrain="6,15,6,12"/>
<tile id="987" terrain="6,12,12,15"/>
<tile id="988" terrain="15,6,6,12"/>
<tile id="989" terrain="12,15,15,6"/>
<tile id="990" terrain="6,12,11,11"/>
<tile id="991" terrain="11,6,12,12"/>
<tile id="992" terrain="12,11,6,6"/>
<tile id="993" terrain="12,6,11,11"/>
<tile id="994" terrain="11,12,6,6"/>
<tile id="995" terrain="6,11,12,12"/>
<tile id="996" terrain="11,6,12,11"/>
<tile id="997" terrain="12,11,6,12"/>
<tile id="998" terrain="6,12,11,6"/>
<tile id="999" terrain="17,12,13,13"/>
<tile id="1000" terrain="13,17,12,12"/>
<tile id="1001" terrain="12,13,17,17"/>
<tile id="1002" terrain="12,17,13,13"/>
<tile id="1003" terrain="13,12,17,17"/>
<tile id="1004" terrain="17,13,12,12"/>
<tile id="1005" terrain="13,17,12,13"/>
<tile id="1006" terrain="12,13,17,12"/>
<tile id="1007" terrain="17,12,13,17"/>
<tile id="1008" terrain="16,17,12,12"/>
<tile id="1009" terrain="12,16,17,17"/>
<tile id="1010" terrain="17,12,16,16"/>
<tile id="1011" terrain="17,16,12,12"/>
<tile id="1012" terrain="12,17,16,16"/>
<tile id="1013" terrain="16,12,17,17"/>
<tile id="1014" terrain="12,16,17,12"/>
<tile id="1015" terrain="17,12,16,17"/>
<tile id="1016" terrain="16,17,12,16"/>
<tile id="1017" terrain="15,16,12,12"/>
<tile id="1018" terrain="12,15,16,16"/>
<tile id="1019" terrain="16,12,15,15"/>
<tile id="1020" terrain="16,15,12,12"/>
<tile id="1021" terrain="12,16,15,15"/>
<tile id="1022" terrain="15,12,16,16"/>
<tile id="1023" terrain="12,15,16,12"/>
<tile id="1024" terrain="16,12,15,16"/>
<tile id="1025" terrain="15,16,12,15"/>
<tile id="1026" terrain="15,6,12,12"/>
<tile id="1027" terrain="12,15,6,6"/>
<tile id="1028" terrain="6,12,15,15"/>
<tile id="1029" terrain="6,15,12,12"/>
<tile id="1030" terrain="12,6,15,15"/>
<tile id="1031" terrain="15,12,6,6"/>
<tile id="1032" terrain="12,15,6,12"/>
<tile id="1033" terrain="6,12,15,6"/>
<tile id="1034" terrain="15,6,12,15"/>
<tile id="1035" terrain="12,11,6,11"/>
<tile id="1036" terrain="6,12,11,12"/>
<tile id="1037" terrain="11,6,12,6"/>
<tile id="1038" terrain="6,11,12,11"/>
<tile id="1039" terrain="12,6,11,6"/>
<tile id="1040" terrain="11,12,6,12"/>
<tile id="1041" terrain="6,11,11,12"/>
<tile id="1042" terrain="11,12,12,6"/>
<tile id="1043" terrain="12,6,6,11"/>
<tile id="1044" terrain="12,13,17,13"/>
<tile id="1045" terrain="17,12,13,12"/>
<tile id="1046" terrain="13,17,12,17"/>
<tile id="1047" terrain="17,13,12,13"/>
<tile id="1048" terrain="12,17,13,17"/>
<tile id="1049" terrain="13,12,17,12"/>
<tile id="1050" terrain="17,13,13,12"/>
<tile id="1051" terrain="13,12,12,17"/>
<tile id="1052" terrain="12,17,17,13"/>
<tile id="1053" terrain="17,12,16,12"/>
<tile id="1054" terrain="16,17,12,17"/>
<tile id="1055" terrain="12,16,17,16"/>
<tile id="1056" terrain="16,12,17,12"/>
<tile id="1057" terrain="17,16,12,16"/>
<tile id="1058" terrain="12,17,16,17"/>
<tile id="1059" terrain="16,12,12,17"/>
<tile id="1060" terrain="12,17,17,16"/>
<tile id="1061" terrain="17,16,16,12"/>
<tile id="1062" terrain="16,12,15,12"/>
<tile id="1063" terrain="15,16,12,16"/>
<tile id="1064" terrain="12,15,16,15"/>
<tile id="1065" terrain="15,12,16,12"/>
<tile id="1066" terrain="16,15,12,15"/>
<tile id="1067" terrain="12,16,15,16"/>
<tile id="1068" terrain="15,12,12,16"/>
<tile id="1069" terrain="12,16,16,15"/>
<tile id="1070" terrain="16,15,15,12"/>
<tile id="1071" terrain="6,12,15,12"/>
<tile id="1072" terrain="15,6,12,6"/>
<tile id="1073" terrain="12,15,6,15"/>
<tile id="1074" terrain="15,12,6,12"/>
<tile id="1075" terrain="6,15,12,15"/>
<tile id="1076" terrain="12,6,15,6"/>
<tile id="1077" terrain="15,12,12,6"/>
<tile id="1078" terrain="12,6,6,15"/>
<tile id="1079" terrain="6,15,15,12"/>
<tile id="1080" terrain="6,6,15,14"/>
<tile id="1081" terrain="15,15,14,6"/>
<tile id="1082" terrain="14,14,6,15"/>
<tile id="1083" terrain="6,6,14,15"/>
<tile id="1084" terrain="14,14,15,6"/>
<tile id="1085" terrain="15,15,6,14"/>
<tile id="1086" terrain="6,15,14,6"/>
<tile id="1087" terrain="15,14,6,15"/>
<tile id="1088" terrain="14,6,15,14"/>
<tile id="1089" terrain="21,21,3,4"/>
<tile id="1090" terrain="3,3,4,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1091" terrain="4,4,21,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="1092" terrain="21,21,4,3"/>
<tile id="1093" terrain="4,4,3,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1094" terrain="3,3,21,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="1095" terrain="21,3,4,21"/>
<tile id="1096" terrain="3,4,21,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RU"/>
	</properties>
</tile>
<tile id="1097" terrain="4,21,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1125" terrain="6,14,6,15"/>
<tile id="1126" terrain="15,6,15,14"/>
<tile id="1127" terrain="14,15,14,6"/>
<tile id="1128" terrain="6,15,6,14"/>
<tile id="1129" terrain="14,6,14,15"/>
<tile id="1130" terrain="15,14,15,6"/>
<tile id="1131" terrain="15,6,6,14"/>
<tile id="1132" terrain="14,15,15,6"/>
<tile id="1133" terrain="6,14,14,15"/>
<tile id="1134" terrain="21,4,21,3"/>
<tile id="1135" terrain="3,21,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="DR"/>
	</properties>
</tile>
<tile id="1136" terrain="4,3,4,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="1137" terrain="21,3,21,4"/>
<tile id="1138" terrain="4,21,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="DR"/>
	</properties>
</tile>
<tile id="1139" terrain="3,4,3,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="1140" terrain="3,21,21,4"/>
<tile id="1141" terrain="4,3,3,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1142" terrain="21,4,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="DR"/>
	</properties>
</tile>
<tile id="1170" terrain="14,15,6,6"/>
<tile id="1171" terrain="6,14,15,15"/>
<tile id="1172" terrain="15,6,14,14"/>
<tile id="1173" terrain="15,14,6,6"/>
<tile id="1174" terrain="6,15,14,14"/>
<tile id="1175" terrain="14,6,15,15"/>
<tile id="1176" terrain="6,14,15,6"/>
<tile id="1177" terrain="15,6,14,15"/>
<tile id="1178" terrain="14,15,6,14"/>
<tile id="1179" terrain="4,3,21,21"/>
<tile id="1180" terrain="21,4,3,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RU"/>
	</properties>
</tile>
<tile id="1181" terrain="3,21,4,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LU"/>
	</properties>
</tile>
<tile id="1182" terrain="3,4,21,21"/>
<tile id="1183" terrain="21,3,4,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RU"/>
	</properties>
</tile>
<tile id="1184" terrain="4,21,3,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LU"/>
	</properties>
</tile>
<tile id="1185" terrain="21,4,3,21"/>
<tile id="1186" terrain="3,21,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1187" terrain="4,3,21,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RU"/>
	</properties>
</tile>
<tile id="1215" terrain="15,6,14,6"/>
<tile id="1216" terrain="14,15,6,15"/>
<tile id="1217" terrain="6,14,15,14"/>
<tile id="1218" terrain="14,6,15,6"/>
<tile id="1219" terrain="15,14,6,14"/>
<tile id="1220" terrain="6,15,14,15"/>
<tile id="1221" terrain="14,6,6,15"/>
<tile id="1222" terrain="6,15,15,14"/>
<tile id="1223" terrain="15,14,14,6"/>
<tile id="1224" terrain="3,21,4,21"/>
<tile id="1225" terrain="4,3,21,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1226" terrain="21,4,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="DL"/>
	</properties>
</tile>
<tile id="1227" terrain="4,21,3,21"/>
<tile id="1228" terrain="3,4,21,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1229" terrain="21,3,4,3">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="DL"/>
	</properties>
</tile>
<tile id="1230" terrain="4,21,21,3"/>
<tile id="1231" terrain="21,3,3,4">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="DR"/>
	</properties>
</tile>
<tile id="1232" terrain="3,4,4,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1260" terrain="18,18,,18">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1261" terrain="18,,18,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UD"/>
	</properties>
</tile>
<tile id="1262" terrain=",18,,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="1263" terrain="18,18,,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LR"/>
	</properties>
</tile>
<tile id="1264" terrain="19,19,,19"/>
<tile id="1265" terrain="19,,19,"/>
<tile id="1266" terrain=",19,,"/>
<tile id="1267" terrain="19,19,,"/>
<tile id="1268" terrain="20,20,,20"/>
<tile id="1269" terrain="20,,20,"/>
<tile id="1270" terrain=",20,,"/>
<tile id="1271" terrain="20,20,,"/>
<tile id="1272" terrain="21,21,,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1273" terrain="21,,21,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UD"/>
	</properties>
</tile>
<tile id="1274" terrain=",21,,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="1275" terrain="21,21,,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LR"/>
	</properties>
</tile>
<tile id="1305" terrain=",18,18,">
	<properties>
		<property name="fence_count" type="int" value="2" />
		<property name="fence0" value="UR"/>
		<property name="fence1" value="DL"/>
	</properties>
</tile>
<tile id="1306" terrain="18,,,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1307" terrain=",,,"/>
<tile id="1308" terrain=",,,18">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="1309" terrain=",19,19,"/>
<tile id="1310" terrain="19,,,"/>
<tile id="1311" terrain=",,,"/>
<tile id="1312" terrain=",,,19"/>
<tile id="1313" terrain=",20,20,"/>
<tile id="1314" terrain="20,,,"/>
<tile id="1315" terrain=",,,"/>
<tile id="1316" terrain=",,,20"/>
<tile id="1317" terrain=",21,21,">
	<properties>
		<property name="fence_count" type="int" value="2" />
		<property name="fence0" value="UR"/>
		<property name="fence1" value="DL"/>
	</properties>
</tile>
<tile id="1318" terrain="21,,,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1319" terrain=",,,"/>
<tile id="1320" terrain=",,,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="1350" terrain="18,,18,18">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="1351" terrain=",,18,18">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LR"/>
	</properties>
</tile>
<tile id="1352" terrain=",,18,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1353" terrain=",18,,18">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UD"/>
	</properties>
</tile>
<tile id="1354" terrain="19,,19,19"/>
<tile id="1355" terrain=",,19,19"/>
<tile id="1356" terrain=",,19,"/>
<tile id="1357" terrain=",19,,19"/>
<tile id="1358" terrain="20,,20,20"/>
<tile id="1359" terrain=",,20,20"/>
<tile id="1360" terrain=",,20,"/>
<tile id="1361" terrain=",20,,20"/>
<tile id="1362" terrain="21,,21,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UR"/>
	</properties>
</tile>
<tile id="1363" terrain=",,21,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LR"/>
	</properties>
</tile>
<tile id="1364" terrain=",,21,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="LD"/>
	</properties>
</tile>
<tile id="1365" terrain=",21,,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UD"/>
	</properties>
</tile>
<tile id="1395" terrain="18,18,18,18"/>
<tile id="1396" terrain="18,18,18,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="1397" terrain="18,,,18">
	<properties>
		<property name="fence_count" type="int" value="2" />
		<property name="fence0" value="UR"/>
		<property name="fence1" value="DL"/>
	</properties>
</tile>
<tile id="1398" terrain=",18,18,18">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
<tile id="1399" terrain="19,19,19,19"/>
<tile id="1400" terrain="19,19,19,"/>
<tile id="1401" terrain="19,,,19"/>
<tile id="1402" terrain=",19,19,19"/>
<tile id="1403" terrain="20,20,20,20"/>
<tile id="1404" terrain="20,20,20,"/>
<tile id="1405" terrain="20,,,20"/>
<tile id="1406" terrain=",20,20,20"/>
<tile id="1407" terrain="21,21,21,21"/>
<tile id="1408" terrain="21,21,21,">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="RD"/>
	</properties>
</tile>
<tile id="1409" terrain="21,,,21">
	<properties>
		<property name="fence_count" type="int" value="2" />
		<property name="fence0" value="UR"/>
		<property name="fence1" value="DL"/>
	</properties>
</tile>
<tile id="1410" terrain=",21,21,21">
	<properties>
		<property name="fence_count" type="int" value="1" />
		<property name="fence0" value="UL"/>
	</properties>
</tile>
</tileset>

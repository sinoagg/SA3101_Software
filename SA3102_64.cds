<distribution version="17.0.0" name="SA3102" type="MSI64">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{A3A33B3B-8BCE-43CA-AB8C-4E075E2109E5}" x64="true">
		<general appName="SA3102" outputLocation="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\cvidistkit.SA3102_1" relOutputLocation="cvidistkit.SA3102_1" outputLocationWithVars="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\cvidistkit.%name_1" relOutputLocationWithVars="cvidistkit.%name_1" upgradeBehavior="1" autoIncrement="true" version="1.0.9">
			<arp company="北京中聚高科科技有限公司" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="English" showPaths="true" showRuntimeOnly="true" readMe="" license="">
			<dlgstrings welcomeTitle="SA3102" welcomeText=""/></userinterface>
		<dirs appDirID="104">
			<installDir name="[Program Files]" dirID="24" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="SA3102" dirID="100" parentID="104" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="hardFileBackups" dirID="101" parentID="104" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="SA3102" dirID="102" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="updates" dirID="103" parentID="104" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="SA3102" dirID="104" parentID="24" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Resource" dirID="105" parentID="100" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="softwareFileBackups" dirID="106" parentID="104" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Resource" dirID="107" parentID="106" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Uir" dirID="108" parentID="106" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Experiment List.uir" relSourcePath="UI Files\Experiment List.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\TerminalPanel.uir" relSourcePath="UI Files\TerminalPanel.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="D:\SA3102\SA3102NEW\SA3102\updates\replace.exe" targetDir="103" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="D:\SA3102\SA3102NEW\SA3102\updates\updateInfo.xml" targetDir="103" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="D:\SA3102\SA3102NEW\SA3102\updates\localInfo.xml" targetDir="103" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\TableData.uir" relSourcePath="UI Files\TableData.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Sample Configuration.uir" relSourcePath="UI Files\Sample Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\GraphDisp.uir" relSourcePath="UI Files\GraphDisp.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="8" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\FET.ico" relSourcePath="Resource\FET.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="9" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Print_pressed.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="10" sourcePath="D:\SA3102\SA3102NEW\SA3102\softwareFileBackups\Resource\Arrow.ico" targetDir="107" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="11" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Arrow.ico" relSourcePath="Resource\Arrow.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="12" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN2.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="13" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Stop.ico" relSourcePath="Resource\Stop.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="14" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN_6.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="15" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Select.ico" relSourcePath="Resource\Select.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="16" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\3102.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="17" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\saveData_pressed.ico" relSourcePath="Resource\saveData_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="18" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\I_I.ico" relSourcePath="Resource\I_I.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="19" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\SMU1.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="20" sourcePath="D:\SA3102\SA3102NEW\SA3102\hardFileBackups\SMU2.0_Code1.bin" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="21" sourcePath="D:\SA3102\SA3102NEW\SA3102\hardFileBackups\SMU2.0_Code2.bin" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="22" sourcePath="D:\SA3102\SA3102NEW\SA3102\softwareFileBackups\Uir\Id-Vds Configuration.uir" targetDir="108" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="23" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Configure.ico" relSourcePath="Resource\Configure.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="24" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN_4.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="25" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN_5.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="26" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\I.ico" relSourcePath="Resource\I.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="27" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Emitter.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="28" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Tools.uir" relSourcePath="UI Files\Tools.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="29" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\EnvironmentDisp.uir" relSourcePath="UI Files\EnvironmentDisp.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="30" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\ProjectPanel.uir" relSourcePath="UI Files\ProjectPanel.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="31" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Id-Vgs Configuration.uir" relSourcePath="UI Files\Id-Vgs Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="32" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Print.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="33" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\I-I.ico" relSourcePath="Resource\I-I.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="34" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\D-ouGraph.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="35" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Sample Analyze.uir" relSourcePath="UI Files\Sample Analyze.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="36" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\I_Bias.ico" relSourcePath="Resource\I_Bias.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="37" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\I-V Configuration.uir" relSourcePath="UI Files\I-V Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="38" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\DoubleGraph.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="39" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\R.png" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="40" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN_7.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="41" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\MainPanel.uir" relSourcePath="UI Files\MainPanel.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="42" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\R-T Configuration.uir" relSourcePath="UI Files\R-T Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="43" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\ReadMe.txt" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="44" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Base.png" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="45" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\R_Bias.ico" relSourcePath="Resource\R_Bias.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="46" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\V-I Configuration.uir" relSourcePath="UI Files\V-I Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="47" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\serialPort.ini" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="48" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\V_Bias.ico" relSourcePath="Resource\V_Bias.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="49" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\analyze.ico" relSourcePath="Resource\analyze.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="50" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\SA3102.png" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="51" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Id-Vds Configuration.uir" relSourcePath="UI Files\Id-Vds Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="52" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\systemClose.ini" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="53" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\saveData.ico" relSourcePath="Resource\saveData.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="54" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\Result Menu.uir" relSourcePath="UI Files\Result Menu.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="55" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\SetPanel.uir" relSourcePath="UI Files\SetPanel.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="56" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\abnormalDataCache.xlsx" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="57" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\I-T Configuration.uir" relSourcePath="UI Files\I-T Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="58" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Graph.ico" relSourcePath="Resource\Graph.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="59" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\icon.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="60" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\SMU2.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="61" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\ToolsBtn.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="62" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Configure_pressed.ico" relSourcePath="Resource\Configure_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="63" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\GND.ico" relSourcePath="Resource\GND.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="64" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="65" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Tools.ico" relSourcePath="Resource\Tools.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="66" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Drain.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="67" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Run.ico" relSourcePath="Resource\Run.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="68" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Save.ico" relSourcePath="Resource\Save.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="69" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\no.ico" relSourcePath="Resource\no.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="70" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Table.ico" relSourcePath="Resource\Table.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="71" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\BTN_3.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="72" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Save_pressed.ico" relSourcePath="Resource\Save_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="73" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Analyze_pressed.ico" relSourcePath="Resource\Analyze_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="74" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\R.ico" relSourcePath="Resource\R.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="75" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Collector.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="76" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\V_Step.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="77" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Gate.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="78" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Table_pressed.ico" relSourcePath="Resource\Table_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="79" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Graph_pressed.ico" relSourcePath="Resource\Graph_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="80" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Bulk.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="81" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\3terminal.png" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="82" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\My Projects.ico" relSourcePath="Resource\My Projects.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="83" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\V_I.ico" relSourcePath="Resource\V_I.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="84" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Pic.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="85" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Select_pressed.ico" relSourcePath="Resource\Select_pressed.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="86" sourcePath="D:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\Source.ico" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="87" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\Resource\My Settings.ico" relSourcePath="Resource\My Settings.ico" relSourceBase="0" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="88" sourcePath="D:\SA3102\SA3102NEW\SA3102\updates\uninst.exe" targetDir="103" readonly="true" hidden="true" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="89" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\UI Files\V-T Configuration.uir" relSourcePath="UI Files\V-T Configuration.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="90" sourcePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\cvibuild.SA31021\Release64\SA3102.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput64 targetType="0" dirID="100" projectID="0">
				<fileID>90</fileID></projectOutput64>
			<projectDependencies64 dirID="100" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="SA31021" targetFileID="43" destDirID="102" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine installToAppDir="false" activeXsup="false" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="false">
			<hasSoftDeps/>
			<doNotAutoSelect>
			<component>activeXsup</component>
			<component>rtutilsup</component>
			</doNotAutoSelect></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WIN7_SP1</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<baselineProducts>
			<product name="NI LabWindows/CVI Shared Runtime 2017" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{28BFBCB6-5EEE-47F1-A894-FCD877057827}" path="(None)" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="17.0.49157">
				<dependencies>
					<productID>{13774AE4-124D-48C2-A8AC-F8070918385E}</productID>
					<productID>{18C5DCAB-EF32-48F8-9789-2A33DE1CA2CB}</productID>
					<productID>{29FBD1D9-6A19-4CF6-935F-C0EFCFB6CA3D}</productID>
					<productID>{2D894BCB-6069-418C-BF94-8E5915D8867C}</productID>
					<productID>{2FC99AB8-F11D-4938-B14F-AC60D1016FE7}</productID>
					<productID>{35E63DC2-6BCC-48D6-A1C5-5B068DF34117}</productID>
					<productID>{4500C77E-71CE-41A9-AD2A-B7D9C2E3397E}</productID>
					<productID>{5E99102B-D07A-4D85-8D50-4BE55E3633A0}</productID>
					<productID>{5FE7B722-6061-4AFA-A26F-2979CB4FEA40}</productID>
					<productID>{68428815-FD93-4897-BC4A-EC5E3194C4E7}</productID>
					<productID>{6EFCD9C0-82FC-4C51-B7F4-3377B6D03BA6}</productID>
					<productID>{92F3B1E4-20BE-44F2-8D2F-F0335FE172A2}</productID>
					<productID>{A4FAF873-938B-43B8-BBF2-E4B2C0CE730B}</productID>
					<productID>{BDADF4B2-9B31-48B3-8E3E-D15047C4C8AD}</productID>
					<productID>{BEA1952D-1F00-4336-9A76-C41FA181C02D}</productID>
					<productID>{CE7D38A4-66ED-4D33-A385-D4A2CF4BFF81}</productID>
					<productID>{D357A74D-E186-4E4B-AF78-E035D3E83368}</productID>
					<productID>{D4E86102-76C1-458C-B0C2-F42B43CAD3CC}</productID>
					<productID>{DA09F5A1-AA07-467D-A0B5-31D9EC8B5048}</productID>
					<productID>{DBAAC357-96E7-459F-B115-B14E29F652EF}</productID>
					<productID>{E2821E0B-CB08-41BD-AE01-A9068F7B8771}</productID>
					<productID>{EAA77133-1A74-45EE-97FB-D67B38720514}</productID>
					<productID>{F05EC50F-B2B4-4B87-A32C-6F9AD11D1B49}</productID>
					<productID>{FE7D71A2-BF64-441C-9489-BE6F6937B98E}</productID>
					<productID>{FF35E2A2-9214-4B76-91AE-5567076AAE8C}</productID>
					<productID>{FF56A0C1-3413-4024-89A6-0FB565B2E78C}</productID>
					<productID>{FF8EF684-CF13-4EE7-BC1D-26AB49EB0CD4}</productID></dependencies></product></baselineProducts>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="d:\SA3102\SA3102NEW\SA3102\SA3102_Software\SA31021.prj" ProjectRelativePath="SA31021.prj"/></Projects>
		<buildData progressBarRate="3.724895399140684">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.221870943359369</ProductsAdded>
				<DPConfigured>0.670717654296872</DPConfigured>
				<DPMergeModulesAdded>1.705524253906242</DPMergeModulesAdded>
				<DPClosed>2.997124335937484</DPClosed>
				<DistributionsCopied>3.141274226562513</DistributionsCopied>
				<End>26.846391451171900</End></progressTimes></buildData>
	</msi>
</distribution>

function Component()
{
    try {
        installer.addWizardPageItem( component, "InstallShortcutsForm", QInstaller.ReadyForInstallation );
    } catch(e) {
        console.log(e);
    }
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    try {
        var isInstallStartMenuShortcutCheckBoxChecked = component.userInterface( "InstallShortcutsForm" ).installStartMenuShortcutCheckBox.checked;
        if (isInstallStartMenuShortcutCheckBoxChecked) {
            if (systemInfo.productType == "windows") {
                component.addOperation("CreateShortcut", "@TargetDir@/murIDE.exe", "@StartMenuDir@/MUR IDE.lnk",
                    "workingDirectory=@TargetDir@");
            }
        }

        var isInstallDesktopShortcutCheckBoxChecked = component.userInterface( "InstallShortcutsForm" ).installDesktopShortcutCheckBox.checked;
        if (isInstallDesktopShortcutCheckBoxChecked) {
            if (systemInfo.productType == "windows") {
                component.addOperation("CreateShortcut", "@TargetDir@/murIDE.exe", "@DesktopDir@/MUR IDE.lnk",
                    "workingDirectory=@TargetDir@");
            }
        }
    } catch(e) {
        console.log(e);
    }

    if(installer.isInstaller()) {
        try{
            if(installer.value("HKCU\\Environment\\PATH", "") == "") {
                component.addOperation("EnvironmentVariable", "PATH", "@TargetDir@\\devkit-x86\\murlibs");
            } else {
                component.addOperation("EnvironmentVariable", "PATH", installer.value("HKCU\\Environment\\PATH", "")+"\;@TargetDir@\\devkit-x86\\murlibs");
            }
        } catch(e) {
        }
    }
}

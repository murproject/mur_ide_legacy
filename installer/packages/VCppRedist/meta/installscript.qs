function Component()
{
    installer.installationFinished.connect(this, Component.prototype.installationFinished);
}

Component.prototype.installationFinished = function()
{
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.execute("@TargetDir@\\vcredist_x86.exe");
            installer.performOperation("Delete", "@TargetDir@\\vcredist_x86.exe");
        }
    } catch(e) {
        console.log(e);
    }
}

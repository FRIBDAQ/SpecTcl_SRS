# comment out first line for xamine
#set DisplayType qtpy
set DisplayMegabytes 200
 
set NonDAQHTTPDPort 6676
set NonDAQMirrorPort 5567

lappend auto_path [file join $SpecTclHome TclLibs]
if {[array names env DAQTCLLIBS]  ne ""} {
    lappend auto_path $env(DAQTCLLIBS)
    package require DAQService
    set HTTPDPort [SpecTcl::getServicePort SpecTcl_REST]
    set MirrorPort [SpecTcl::getServicePort SpecTcl_MIRROR]
} else {
    set HTTPDPort $NonDAQHTTPDPort
    set MirrorPort $NonDAQMirrorPort
}

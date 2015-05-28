Now that a functional foundation of the source code is available, it would be great to get this repository active again with volunteers who would add to this project! Here are some goals to start us off...<br>
<br>
<b>1.</b> <b>Visual Studio 2010, 2012, and 2013</b>: Creating versions of Solution and Project files for building with these later versions of Visual Studio.<br>
<br>
<b>2.</b> <b>DirectX SDK more recent than Aug 2007</b>: Updating the source code to work with more recent versions of DirectX.<br>
<br>
<b>3.</b> <b>Releasing Hidden Functionalities</b> of DarkBasicPro into DarkGDK. Now that the source code for both has been integrated, there is much functionality within DarkBasicPro that has not yet been switched on with DarkGDK. Much of this can be done by creating simple wrapper functions. So much can be accomplished here in very short time.<br>
<br>
<b>4.</b> <b>New Documentation</b> that is complete, accurate, up-to-date, inclusive of all the functions along with their purpose, usage, options, and examples that are complete, practical, and concise.<br>
<br>
<b>5.</b> <b>Cleaning & Reorganisation of the source code</b>.<br>
<ul><li>This includes the Solution/Project structures. At present, the DarkGDK and DarkBasicPro Solutions have several projects, many of which include the very same files, so for a single build, some of the files get compiled several times across the different projects.<br>
</li><li>In the DarkGDK library includes, some of the .h files have prototypes to internal function prototypes that are not meant to be used directly in DarkGDK gaming projects. These need to be moved elsewhere.<br>
</li><li>Several function prototypes are duplicated in different .h files. These all need to be in a central location so there are no duplicates.<br>
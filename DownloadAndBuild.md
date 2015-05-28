To compile the latest version of the source code ([r111](https://code.google.com/p/darkbasicpro/source/detail?r=111) or higher), instructions for downloading the source code are here:<br>
<a href='https://code.google.com/p/darkbasicpro/source/checkout'>https://code.google.com/p/darkbasicpro/source/checkout</a><br>
<br>
To build the source code, you will need:<br>
Visual Studio 2008<br>
DirectX SDK (Aug 2007)<br>
Windows XP (later versions have not yet been confirmed)<br>
<br>
<b>DarkGDK - To build libraries:</b><br>
1. Go to <b>DarkGDK\Code\Compiler - Visual Studio 9\</b><br>
2. Run <b>Dark SDK.bat</b> to copy a few files so the Solution can see them.<br>
3. Open <b>Dark SDK.sln</b>, and click build.<br>
<br>
The compiled libraries for DarkGDK gaming projects will be in <b>DarkGDK\Code\Lib\</b>.<br>
The accompanying includes are in <b>DarkGDK\Code\Include\</b>.<br>
<br>
You no longer need to link strmbase.lib and strmbasd.lib in your DarkGDK gaming projects.<br>
<br>
Functionality of the compiled DarkGDK libraries has been tested, although not every aspect thoroughly.<br>
<br>
<b>DarkBasicPro - to build libraries:</b><br>
1. Go to <b>Dark Basic Pro SDK\DarkSDK\</b><br>
2. Open <b>DarkBasicProSolution.sln</b>, and click Build.<br>
<br>
There are several build configurations to choose from, to include various 3rd party devices, such as the VR-920 virtual reality headset. The SDKs for these products is required to already be present on your system to be able to complete these builds. These alternative builds may still have some flaws in their project settings, as only the default configuration has been recently tested for building.<br>
<br>
Functionality of the compiled DarkBasicPro libraries has not been tested.<br>
<br>
<b>Warning</b>: Before you build DarkBasicProSolution.sln:<br>
These projects create outputs here (Animation, Camera, ConvX, Input, Setup / Setup - VR920, System, Text):<br>
C:\TGC\CODING\Dark Basic Pro Shared\Dark Basic Pro\Projects\BIGROCKS<br>
<br>
These projects create outputs here (Bitmap, Image):<br>
C:\Program Files (x86)\The Game Creators\FPS Creator\MyGames\simple<br>
<br>
These projects create outputs here (DBDLLCore):<br>
C:\Program Files (x86)\The Game Creators\FPS Creator\MyGames\fpscgame<br>
<br>
These projects create outputs here (Objects, ODE):<br>
Note: If you have FPS Creator installed, these outputs will likely break it!<br>
C:\Program Files (x86)\The Game Creators\FPS Creator<br>
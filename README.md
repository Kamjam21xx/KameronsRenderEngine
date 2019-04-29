# KameronsRenderEngine
OpenGL render engine

Here is a link to my portfolio. There is a blog tab that ill be adding too, strictly for graphics programming.
https://kamjam66xx.wixsite.com/briggsmedia


i will be adding documentation when it gets more complete. i imagine ill eventually just have a high level api for games/graphics. 
i plan on adding my audio and physics engine after the graphics are solid.




//<>=====<> will be adding this to a .md <>=====<>// moving to-do list here for a little while


/*	 gotta give thanks to the Graphics Programming discord group for helping me learn and fix things when im stuck

	FYI all hand typed, no auto get/set etc

	to do list
	- CODE REVIEW! when most of this is done

	x eliminate direct paths
	x encapsulate loading for skybox and simplify functions/construction
	x encapsulate skybox inside scene class
	x check and eliminate win lib include
	x setup FrameBuffer class
	x add GetSkyBox() to the scene class and fix implementation in main
	x simplify RenderPass() by moving part of it to a new function in main "RenderToQuad()"
	x added gamma slider and uniform 
	x added DualFrameBuffer class ..... might be a bad call, but it keeps the standard FrameBuffer class easy to use and simple, as well as other things
	x implement GUI for changing multisampling and for turning on and off v-sync


	//////////   D   O   N   T      F   O   R   G   E   T   //////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	- IMPLEMENT RULE OF 5       wasted 2 days from not implementing or being ignorant of the copy constructor
	- optimize ---- getting choppy results in terms of updating the 0'th framebuffer

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////



	- use v to mark done instead of x / move to a todo.md on github 
	- add thread handler / make multithreaded (at a later date)
	- automate more hardcoded numbers
	- add resolution setting
	- add start menu
	- change default camera direction and position, so the demo starts viewing the entire scene at a 45
	- add configuration class
	- make DualFrameBuffer class into MultiFrameBuffer class, and make it more automated
	- add post process prebuilt shader buffer and select different shaders based on ImGui selection
	- improve and standardize naming conventions as youre working through stuff
	- eliminate wasteful pass by values and other odds and ends
	- change include paths - per deccers advice
	- fix error checks printf %s with 2 parameters is wrong - per deccers advice
	- stop xChange and yChange for mouse input while not clicked, if first clicked, change is 0.0f
	- make materials encapsulate references to textures 
	- make texture packing functionality for faster gpu stuff 
	- eliminate matrix multiplication in vertex shader for faster execution speed - per jodies advice
	- make the scene class encapsulate more to clean up the code and make it more understandable
	- finish shader baker class "ShaderHandler" and work it in
	- setup more GUI functionality 
	- make GUI more efficient and pretty with tabs
	- clean shaders
	- pack texture data into fewer texture units with no unused channels
	- add metal to scene, model, texture, shaders and shader class
	- fix directional light and fix to camera
	- fix spotlight edge calculations
	- setup hdri and post processing buffer
	- setup deffered rendering
	- add pbr mode, pipeline, shaders, and GUI manip
	- implement fast fourier transform
	- implement full reflection reflection
	- implement multiple shaders and shader changing in the GUI
	- reinstate alpha and blending and add sort algorithm and seperate shader 
	- implement auto alpha detection for if the alpha channel is used, and use correct texture loader. flip bit flag for hasAlpha
	- add mouse smoothing WHILE CONTROLING CAMERA
	- add particles
	- add voxel based volume rendering
	- add voxel based fluid simulation
	- add particle simulation
	- create water shader with FFT
	- add hotkey/keyBinding editor
	- add smooth acceleration to camera movement
	- add sort algorithm for baked shader functionality
	- add audio engine
	- add event handling
	- add physics engine
	- possibly implement extra shaders to avoid hacking good shaders and not fixing them	



	CLEAN UP MAIN
	+ much more

*/

/*
	making changes thanks to code reviews. if they arent here yet, they will be. 

	will add license info, plz dont sue me or steal my project.0
*/

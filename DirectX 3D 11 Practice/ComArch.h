#pragma once

/*Direct 3D Fundamentally built off of COM.
  Though Direct3D abstracts
  
  C++ is designed to reuse objects via source code, not binaries.
  C++ doesn't dictate the layout of binary data.

  In C++, It's difficult to have binaries interacting if they were 
  produced by different compilers as compilers will generate 
  different binaries from the same source code. Even different
  versions of the same compilers will produce binaries with subtle
  differences. 
  
  COM provides a standard for binary compatibility. We can have 
  binaries on the machine that can be used by other software. 
  
  To understand COM you must understand INTERFACES:
	Problem: You have a function punch that works on trolls
	You want to punch a chest, so you create a base class 
	punchable for both orcs and chests. While this solves the 
	problem, it increases the depth of the inheritence tree, 
	which complicates the code. 
	
	Solution (at least in OOP) is to create an interface that
	can be implemented by multiple base classes. Anything that
	signs with an interface gets access to a certain suite of 
	functions. 
	
Direct 3D Architecture

Device: Could be physical graphics device or an emulation layer.
*/
---
layout: post
title: 'Week 2: On memory management'
date: '2020-09-13 16:00 +0200'
author: Albert Rodriguez
published: true
categories: DevJourney
---
Last week I was telling you about how Vulkan's most highlighted features was memory management and synchronization. This week I'd like to elaborate a little more memory after this week working with LeapOfFaith.

# An always hot topic
If you have ever worked in the software development industry, no matter the language or environment you've most likely heard about memory management. In particular an always present mandra on the matter could be `beware of memory allocations, try not to allocate memory at runtime unless it's necessary`. But you might wonder, why in 2020 with tons and tons of RAM and fast processors is it so hard to reserve memory? There are a few reasons for it but the main idea is that requesting new chunks of memory at runtime requires a lot of context switching between the operating system and the application requesting it, and the more memory addresses to use the more complex it gets... Depending on the language you'll see this problem tackled in different ways. 

In the case of mainstream frameworks such as C# or Java you have a virtual machine that keeps track of the objects you've allocated and contains a process called `Garbage Collector` which observes how the code references to those chunks of memory and deallocates when there are no more references to it. This on paper looks nice but has a few drawbacks, the main one is that garbage collection is computationally expensive and in some cases requires the whole application to stop running  until the process has finished (famously known as stop-the-world). That's the reason why new frameworks (such as ASP.NET Core) try to allocate everything on the scope of the current function being executed to gain speed.

In the case of `closer to metal` languages such as C/C++ or Rust we have the typical pointer mechanism which in turn has been improved with smart pointers which are a more lightweight way of garbage collection.

# Memory management in Vulkan
TODO

# Recommended abstractions
TODO

# Where am I on LeapOfFaith?
After a few days non-stop I finally managed to start rendering meshes with textures! 😊

<div style="text-align: center">
  <img src="{{site.baseurl}}/assets/drawing-meshes-and-textures.gif" width="80%" />
</div>

And I managed to finish the Udemy course! I have to admit I'll need to revisit some of Vulkan's key concepts as this was a lot of information to take in but so far glad I'm starting to see the end to end side of things!

# What's in for next week?
The idea is to review some of the key topics of Vulkan along with:
* Re-enable the valiation layers
* Tackle suggestions from Marco (my Vulkan mentor)
* Improve C++ project structure
* Start thinking about refactorings and abstractions (at the moment I only have)

See you next week!
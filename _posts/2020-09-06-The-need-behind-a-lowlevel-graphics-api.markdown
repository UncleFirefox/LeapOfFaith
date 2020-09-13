---
layout: post
title: 'Week 1: The need behind a low-level graphics API'
date: '2020-09-06 23:13 +0200'
author: Albert Rodriguez
published: true
categories: DevJourney
---
For this week's journey there's something that's been behind my mind on my jump-start in Computer Graphics. As I mentioned on a previous post, my idea is to have an accelerated mentorship in Vulkan. As a developer I always try to understand the reason behind investing in certain tech. A reason which is always accompanied by a need in the market. In this article I'll try to give my perspective on that need.

# Vulkan on the surface
Anybody who is a software developer or a graphics enthusiast still remembers when Vulkan became publicly available around [2016](https://www.techradar.com/how-to/gaming/what-is-amd-vulkan-and-what-does-it-mean-for-the-future-of-gaming-1323469). 

Many articles at the time gave the same argument: closer to the hardware, more performance. That idea can be understood by a broader audience, however, when it comes to a software engineer, curious by nature and willing to learn, things cannot be so obvious... `So what does this really bring to the table?` To answer that question probably have to ask ourselves `What's on the table already?`

# The origins of Computer Graphics Standards
If you've done computer graphics before chances are you've worked with OpenGL. At its core OpenGL is nothing more than a set of standardized function calls that allow you to interact with your graphics card, in other words, OpenGL defines an abstracted behavior and expected results and the different hardware vendors (NVIDIA, AMD and Intel) implement and expose those functions through you graphics card driver which in turn are called by the game/application you are executed on your hardware.

From a Software Engineering perspective, abstractions are one of the most necessary parts in a system, especially if you work with complex machinery such as a GPU. The hard part in abstractions is to make the right choices at the right time. 

In the case of OpenGL we're talking about a journey that started in 1992 when GPUs were very specialized hardware for professional studios (that was 2 before the first PlayStation was released, insane!). And over time the OpenGL standard kept evolving trying to adapt to the new innovations in the market of GPUs. 

All over the years one might expect that keeping compatibility with existing graphics cards and standards must've been hard. We all have evidence of that. Who being in the PC gaming area hasn't heard about specific game driver patches for their graphics card? The driver... Big problems always came down to the fact that it was not your hardware, it was the software that ran on top of it that caused them.

# The bottleneck
As time passed, GPUs started being more and more complex. They started to tackle problems that were traditionally for general-use CPUs (e.g. CUDA or OpenCL) or incorporated dedicated hardware for computaionally-intensive tasks (e.g. NVIDIA and the inclusion of Tensor Cores or the recent Ray-Tracing cores) which opened for more and more possibilities within the computer graphics industry.

OpenGL was not designed with all these innovations in mind and although it has adapted to some of them, there are major problems on the stack at its core that prevent it from fully utilizing the real hardware. 

The biggest one is the abstraction it has on the graphics pipeline which is also called as OpenGL state. The goal is to essentially maintain some kind of state machine with dozens of variables that cause the system to do the things you expect according to the OpenGL specs so how do those change in variables tell the graphics card to do the stuff we want? You probably guessed it... the driver! So on top of your OpenGL graphics application there is a lot going on behind the scenes with the driver translating to real hardware not there could be bad programmers on both sided of the equation (the application and the driver) which is why in some cases gpu vendors need to release patches for specific games or games need to release specific patches for cards.

Can you image a scenario like that with the innovations we're going to see on the next generation of games? Take for instance the features of the upcoming [Unreal Engine 5](https://www.youtube.com/watch?v=Oa2drgVThbs) with billions of ray-traced triangles. From the second half of the past decade it became quite obvious that the mixture of bad core-engineering decisions and the increase of complexity in software and hardware required a new paradigm shift in computer graphics APIs.

# Vulkan, a more realistic abstraction
So the idea behind Vulkan is simple: Give more control to the developer in every aspect of the graphics card. We can immediately see two sides:
1. The resources of the graphics card are better utilized: Rather than keeping some enormous state machine and expect some third party software to interpret it and communicate with real hardware, how those interactions occur are now up to the developer and the concrete use-case.
2. You need to write more code: Which is why many people dislike Vulkan. To be fair I can't blame them, we're talking about writing code that in the past was sitting behind the scenes inside our graphics card driver. Compared to OpenGL we're talking about code regarding memory management and synchronization. One could write up to a thousand lines of set-up code to render a triangle.

To give you an idea of all the aspects of Vulkan, look at the following diagram:

<div style="text-align: center">
  <img src="{{site.baseurl}}/assets/Vulkan-API.png" width="80%" />
</div>

Before you run away scared of Vulkan's complexity, think about it for a second... If you were really interested in having the best performance (in essence, what a graphics card can really do), shouldn't you really care about the hardware your software is running on top of? When I started looking at Vulkan I couldn't understand why now more complexity is pushed to the developer but I ended up seeing that it is really a necessity and a price worth paying. Other APIs like the broadly used DirectX from Microsoft did a simillar move on version 12 [for the same reasons](https://www.pcgamer.com/what-directx-12-means-for-gamers-and-developers).

# So what's the best way to learn Vulkan?
I think that Vulkan is the perfect opportunity to get back to hardware concepts. You should combine the look at how a graphics card currently works nowadays with how the API exposes it. I recommend three different sources:
1. A course at Udemy [Learn the Vulkan API with C++](https://www.udemy.com/course/learn-the-vulkan-api-with-cpp)
2. Sacha willems [github repo](https://github.com/SaschaWillems/Vulkan) full of Vulkan code examples
3. The [official docs](https://devdocs.io/vulkan/) on Vulkan, which is really complete and full of resources.
4. The book [Vulkan Programming Guide](https://www.amazon.com/-/es/Graham-Sellers/dp/0134464540/)

# Where am I on LeapOfFaith?
Currently developing the foundations of the engine, mainly understanding the abstractions of Vulkan as opposed to OpenGL. I really recommend the Udemy course where you can follow along quite easily. Fortunately after around a thousand lines of code I got the first triangle on screen 😊 

<div style="text-align: center">
  <img src="{{site.baseurl}}/assets/triangle-on-screen.png" width="80%" />
</div>

# What's in for next week?
The idea is to finish the Udemy course and have all the important parts assimilated and developed:
* A basic camera system
* Mesh rendering
* Textures

See you next week!
# Comparison of ASP.NET MVC and Node.js for Backend Programming

We will compare ASP.NET and Node.js for backend programming.

### About ASP.NET

ASP.NET  is a web application framework by Microsoft.
ASP.NET offers common functionality for others frameworks on top of it:
ASP.NET Web Pages, ASP.NET Web Forms, ASP.NET MVC, etc.
For example, ASP.NET includes such common functionality as facilities for managing requests, handling sessions and a login security model based around membership.
These frameworks has different goals, may be used together and will be merged into MVC 6 or ASP.NET 5.
Sources: [asp.net](http://www.asp.net/get-started/websites).

### Processing Models

The main difference between Node.js and ASP.NET frameworks is their processing models.
Node.js dictates asynchronous processing style while ASP.NET offers programmer a choice.
Node.js was build around asynchronous model from ground up while `async` keyword appeared in ASP.NET MVC 4.
Figures below demonstrate differences between synchronous and asynchronous models for a web application.

![Node.js processing model](https://strongloop.com/wp-content/uploads/2014/01/threading_node.png)

As shown Node.js uses one thread for handling requests and many threads to provide asynchronous non-blocking IO (e.g., to the database or to other REST server). Fewer threads means fewer memory for stack allocation and more economic usage of the CPU.

![enter image description here](https://strongloop.com/wp-content/uploads/2014/01/threading_java.png)

In this multi threaded model the server spawns new thread for handling each request which sleeps on blocking IO operations consuming CPU and memory resources.

So how exactly does ASP.NET work? ASP.NET doesn't use one thread but instead uses restricted number of threads from the pool and queues requests to it. Threads may be terminated on asynchronous operations like in Node.js. However, ASP.NET processing model is more prone to context switching which implies additional CPU costs. More than that as ASP.NET and .NET weren't designed with asynchronous programming in mind some libraries may still offer no support for it making its freedom of choice quite restricted.

Sources:
[What Makes Node.js Faster Than Java?](https://strongloop.com/strongblog/node-js-is-faster-than-java/)
[Node.js Thesis: IO is Expensive](http://blog.mixu.net/2011/02/01/understanding-the-node-js-event-loop/)
[ASP.NET Thread Usage on IIS 7.5, IIS 7.0, and IIS 6.0](http://blogs.msdn.com/b/tmarq/archive/2007/07/21/asp-net-thread-usage-on-iis-7-0-and-6-0.aspx)
[Node.js and Context Switching](http://stackoverflow.com/questions/16707098/node-js-kernel-mode-threading).
[Opinionated Node.js vs. Non-opinionated ASP.NET](http://stackoverflow.com/a/11060092/521957).


### Programming Languages

ASP.NET uses C# as its primary language.  
Node.js uses JavaScript and all the languages that can be transpiled to it like CoffeeScript, Microsoft TypeScript and recent EcmaScript2015 (aka ES6).

Without doubt C# is a more robust language than JavaScript.
C# offers strict type system and compile-time error checks and in JavaScript you may get it through Facebook Flow static type checker or Microsoft TypeScript.
C# has classical inheritance model, EcmaScript6 classes offer new syntax for the same prototypical inheritance (which is claimed to be not so powerful).

However some developers are in fear of .NET software patent system, e.g.:
- [Why free software shouldn't depend on Mono or C#, Richard Stallman](https://www.fsf.org/news/dont-depend-on-mono),  
- [Comparing Java to .Net](http://en.swpat.org/wiki/Comparing_Java_to_.Net_and_C-sharp).
I don't see any threat from JavaScript software patents.

JavaScript is a more ubiquitous language though to master Node.js the developer is also required to be acquainted with asynchronous programming style.

### Support for Asynchronous Programming

Both, C# and JavaScript support asynchronous programming.
C# exerts `async` and `await` keywords.
As a pioneer Node.js proposed several approaches to asynchronous programming:

- Using callbacks. This way your code becomes hard to read, and control flow is hardened. See Node.js Pyramyd of Doom for examples. This approached appeared the first. 
- `yield` keyword, co-routines, generators and Promises eliminate your code from deep nested callbacks and makes it easier to read. This approach is demonstrated by Koajs web framework.
- `await` and `async` keywords are part of experimental EcmaScript7 standard and may be used today with transpilers like Babel. This syntax constructs make your code more clear for those not introduced to Promises and generators.

So, C# asynchronous model is more clear.
Node.js is slightly different though:

1. It compels you to write your code in asynchronous manner, so most libraries support asynchronous model.
2. It implies fewer context switches as stated by some sources.

### Abstractions and Conventions

Node.js is "close to the metal." It offers fewer and thinner abstractions. So you don't have to configure their overwhelming number of parameters, but instead you are faced with writing the code out of multitude small components. This approach gives you flexibility of tailoring the code up to the solution. It may be expected that in place of configuration the programmer is supposed to write much more boilerplate code. It's not the case with Node.js as it offers all needed facilities from native or third party libraries at the same time not compelling you to use bloated abstractions. Node.js programmer is free in choice of programming libraries, has a uniform way of combining them as middlewares, may easily divert from MVC in favor of any other architectural pattern and controls options that may be concealed by abstractions in other frameworks. Node.js imposes only a few conventions: asynchronous programming, most servers and frameworks use middlewares (Express.js, Koa.js).

On the other hand ASP.NET MVC is quite different. It offers abstractions but doesn't imply much configuration or boilerplate code. Instead it leverages convection other configuration principle. It means it has conventions which are when followed make your code concise and readable. E.g. there is a predefined folder structure for every MVC project to follow, naming conventions for controllers, etc. So, yes, ASP.NET imposes upon you its view of things. And if you want to divert from conventions then you are deprived of all this automatic out-of-the-box behavior and faced with manual configuration. Now, the question is whether these conventions are good and apt for your tasks, will they make you happy with your task so you don't have to configure everything from scratch.

Conclusion:
- ASP.NET exerts many conventions with which everything works out of the box, but tailoring code for your needs requires much configuration.
- ASP.NET offers many abstractions which may be great for large applications but seem bloated for simple tasks. Also programmer looses sense of control when everything is automated behind abstractions.
- Node.js imposes very few conventions but offers great flexibility.


### Performance

Certainly there are tasks where ASP.NET outperforms Node.js and there are tasks where ASP.NET looses.
To take advantage of multi core system Node.js must be clustered.
To outperform with IO-expensive tasks ASP.NET must use `async`/`await` keywords.

Here is a chart benchmarking ASP.NET Web API and Node.js in 2012, [source](http://mikaelkoskinen.net/post/asp-net-web-api-vs-node-js-benchmark-take-2).

![ASP.NET Web API vs Node.js](http://adafyservicesstorage.blob.core.windows.net/mikael/image_76.png)

Chart below compares performance of Node.js and ASP.NET MVC 4 in 2012, [source](http://rarcher.azurewebsites.net/Post/PostContent/19).

![Node.js vs MVC 4](http://rarcher.azurewebsites.net/Images/nodeMvc30.png)

The explanation why asynchronous Node.js is faster than asynchronous ASP.NET may be that it uses fewer context switches.

Node.js is created for fast request handling without heavy computations. In all tasks requiring heavy computations Node.js will certainly loose to ASP.NET.
In cases where Node.js is not clustered it looses to ASP.NET, e.g. [see this](http://stackoverflow.com/a/10641377/521957).


### Portability

Except some issues Node.js works great on all major platforms.
ASP.NET is ported partially, e.g. MVC4 on Mono lacks support for async features.
The forthcoming MVC6 is expected to cover all major platforms.
Definitely, Node.js wins in portability.
Sources: [Mono Compatibility](http://www.mono-project.com/docs/about-mono/compatibility/).

### Reliability

Reliability is defined by how robust your program to failures in input, runtime exceptions, programming errors, etc.
C# is more robust as it offers strict-type system which JavaScript lacks.
Things get more complicated when dealing with error handling in asynchronous code. It's more robust to use generators with `yield` wrapped in `try`/`catch` instead of error callbacks in Node.js.
Some programmers are not happy with JavaScript being not robust, e.g. [see this](https://medium.com/@tjholowaychuk/farewell-node-js-4ba9e7f3e52b).


### Learnability

Learnability is defined by how easy it is for a newcomer to pick up the language.
Being dynamically typed, JavaScript is easier to pick up, but in Node.js to deliver high-quality code you eventually has to know Promises, generators and co-routines. New syntax sugar of upcoming EcmaScript standards like ES2015 and ES7 add more ways to express the same meaning making non-recent code less readable.

### Ecosystem

By ecosystem I mean the availability of third party packages and community support.  
As Node.js is more portable its community is wider. JavaScript is also more ubiquitous than .NET. Provided with the idea of louse coupling Node.js ecosystem is rich and robust.

### Conclusion

Java outperforms C in overwhelming majority of aspects. But if you have to write high-performance code, e.g., system driver, or independent project then Java is no choice in favor of C.
The followig table summarizes comparison.

\+ means "wins"
\- means "looses"
| Characterisitc     | Node.js   | ASP.NET
| ----------------   |:------:   |:-------:
| IO Performance	 | +		 | -
| Computational Perf.| -		 | +
| Asynchronous pr-ng | required	 | possible
| Languages		     | JS looses | C# wins
| Portability		 | +		 | -
| Reliability		 | -		 | +
| Ecosystem		 	 | +		 | -
| Learnability		 | +	 	 | -
| Simplicity		 | +	 	 | -
| Flexibility		 | +	 	 | -


### Some materials used
[To Node.js Or Not To Node.js](http://www.haneycodes.net/to-node-js-or-not-to-node-js/)
[Is Node.js better than ASP.NET?](https://thomasbandt.com/is-nodejs-better-than-aspnet)
[The Node.js Philosophy](http://blog.nodejitsu.com/the-nodejs-philosophy/)
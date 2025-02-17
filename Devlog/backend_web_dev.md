### What I learned about Backend Web Dev

#### Rest APIs
links used:
    - [rest api in 100 seconds](https://www.youtube.com/watch?v=-MTSQjw5DrM)
    - [learn expressJS in 35 minutes](https://www.youtube.com/watch?v=SccSCuHhOw0&t=909s)
    - [learn express middleware in 14 minutes](https://www.youtube.com/watch?v=lY6icfhap2o&t=23s)
    - [Important HTTP status codes](https://blog.webdevsimplified.com/2022-12/http-status-codes/)
- **General background**
  - an api is a way for computers to talk to eachother; essentially a methodology to handle routing in a web-app i.e. communicating and sending resources between the frontend and backend. This is good for isolating important data from the client, and allows for the client to dynamically recieve data which can create a bespoke experience.
  - a *rest* api is a type of api that uses URI's (like urls) endpoints that the client can make *requests* to, and include a *payload* (typically a json file), and recieve *responses* from the server.
    - "resources" typically refer to data structures and files
  - common rest api verbs:
    - *get*: read resource from backend
    - *post*: create resource in backend (replacing existing resources when necessary) typically using resource payload to specify creation
    - *put*: replace target resource with request payload
    - *patch*: partially modify a resource
    - *delete*: delete resource specified
- **HTTP status codes**:
  - http status codes are sent as part of the server response
  - common status codes:
    - 2** (Good)
      - 200: Ok
      - 201: Created
      - 202: Accepted (response recieved, but request not completed yet)
      - 204: No content (response is empty)
    - 3** (Redirecting)
      - 301: Moved Permanently (page at a certain URL has been permanently moved to a new URL)
      - 302: Found (page is at a new URL but this is a temporary change; search engines wont recognize it as the default)
      - 304: Not Modified (used for caching and essentially just says that the resource being requested has not changed)
        - used in conjunction with a previous 200 status request that included caching headers
    - 4** (Client Error)
      - 400: Bad Request (Indicates a bad payload)
      - 401: Unauthorized (not logged in or wrong credentials)
      - 403: Forbidden (client doesn't have permission)
      - 404: Not found (e.g. access a URL or database that doesn't exist)
      - 429: Too many requests (rate limit of API has been reached)
    - 5** (Server Error)
      - 500: Internal server error (generic catch-all)
      - 503: Service unavailable (e.g. server being down for maintenance)
        - This should also include a Retry-After HTTP header with the estimated time until the server will be back up.
  - list of HTTP request headers: [from MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers)

#### Express JS (API I'm using)
- Express JS is a library for building a rest api with a node.js backend
- you can statically serve files (useful for html files that don't change) easily
  ```js
    // it's standard for the parent dir to be called "public"
    app.use(express.static(path.join(__dirname, "public")));
  ```
  - the files in `/public` are served based on their relative path in the public folder
- you can dynamically serve files (useful for web pages that change, and redirecting users):
  - you have to have a "view engine" set up
    ```js
      app.set('view engine','ejs')
    ``` 
    - [what is a view engine?](https://stackoverflow.com/questions/8308485/what-is-view-engine-what-does-it-actually-do) It's a program that creates html from your views
    - views are typically html + a programming language
    - the ejs engine is javascript-like, so the tutorial recommends it.
    - NOTE: I will probably not be using views for this project, since the frontend can easily be represented statically
  - here is an example render request in the server:
    ```js
      //app.get("/",(request,response,next)) : don't care about next most of the time
      app.get("/dynamic",(request,response) => {
          response.render("index",{text : "Caleb"});
      })
    ```
    - the get request (the default request) at the relative url "/dynamic" renders the file in the views folder called index.ejs, and passes in the object in the second param "options"
  - here's an example of an html body with ejs code that runs when the resource request is called (enclosed in <% %>):
    ```html
      <body>
        Hellow  <%= locals.text || "default" %>
      </body>
    ```
    - "locals" is an object that allows the .ejs file code to access the "options" passed in the render request, so that looking for the value at .text will either be defined or undefined, rather than producing a rendering error
- express allows for similar routes to be modularized
  ```js
    const express = require('express');
    const router = express.Router();
    // ... your endpoints here
    module.exports = router
  ```
  - this basically creates a mini api app that all the routes declared in this file will use, that then gets sent to the main api
  - if this route was "./users" all the endpoints would implicitly start with that
- express has .route function which allows common rest endpoints to be chained together
  ```js
  router.route("/:id")
      // note: in javascript: template strings are surrounded by backticks
      // note: ${JSON.stringify(req.username)} formats an object in the string
      // note: :id catches everything after the / as the request parameter "id", it is a DYNAMIC ROUTE
    .get((req, res) => {
      res.send(`get ${JSON.stringify(req.username)} with ID ${req.params.id}`);
      })
    .put((req, res) => {
      res.send(`update users with ID ${req.params.id}`) 
      })
    .delete((req, res) => {
      res.send(`delete users with ID ${req.params.id}`) 
      })
  ```
- express has lots of built in middle-ware, and support for these
  - middle-ware is any functions that are executed in between the request and response
  ```js
    router.use(logger) // callback function to middleware
    // note: since declarations are sequential, this middleware is applied to ALL "router" endpoints declared AFTER it. But the function can be declared after it's called.
    function logger(req,res,next){
      console.log(req.originalUrl)
      next()
      // next is generally needed in middleware, to tell the server to continue routing the user from the request
    } 
    router.route("/:id")
    // example of passing middleware as callback function; runs logger() an extra time
          .get(logger,(req, res) => {;
            res.send(`get ${JSON.stringify(req.username)} with ID ${req.params.id}`);
            })
  ```
  - example of built in middle-ware:
    ```js
      // allows server to access form responses
      app.use(express.urlencoded({extended:true}))
    ```
  - express allows us to define query params
    - e.g. if a user entered "GET /?name=Caleb" it is sending a query request with the payload `name="Caleb"`
    - the server can handle this like so:
      ```js
        router.get("/", (req, res) => {
          //accesses query parameter with req.query.name, and uses it to do a dummy console.log to tell the user whether this string is in an array
          console.log(users.find( (element) =>  element.name === req.query.name))
          // redirects the user, sending them on their merry way
          res.send("/users")
        })
      ``` 
  - the .params() function asynchronously adds parameters to dynamic endpoints that match the variables declared in it
    ```js
      router.param("id",(req,res,next,id) => {
        // adds "username : string to the request object
        req.username = users[id];
        // continues the program
        next();
      })
    ```


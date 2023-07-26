# WebServ

An https server written in c++98 capable of hadeling POST, GET and DELETE. Developed as pert of the ecole 42 cursus.

## Config file

To configure the server we need to abhear to the rules outline in the doc.

The config file is strucrued like so:

```bash
server billy {
	listen = 8080
	server_name = something www.something.com
	location / {
		alias = html
	}
}
```

### Redirections
It is possible to configure client HTTP redirects using the `redir` directive.
Example:
```
server test-server {
	listen = 8080
	server_name = test
	location /redirected {
		redir = https://www.google.com temporary
	}
}
```
It is possible to configure a permanent redirect using `redir = https://www.google.com permanent`.
The difference between `temporary` and `permanent` is the HTTP status code that is returned to the client.

### config file parsing

The config file is parsed using recursion and the Data class as the singular node that holds data. The hierarchy produced from parsing is what later needs to be interogated to configure the server.

```c++

typedef std::pair<std::string, Data> dataObj;  //pairs of object name and the object
class Data
{
    [...]
    std::string _content;                      // the contents of the node as string
    std::vector<dataObj> _vecObjs;             // a vector of pairs
    [...]
}
```
After parsing internal structure is aproximated to this ...

Argh this is getting to a a mess to try show, grossomodo there are two types of data bits.

A straight forward property `ip = 192.168.1.1`
A container of more properties `ports {`

If it's a straignt forward property look at the content to get the `192.168.1.1` and the parent nodes string pair for the property name `ip`. Same goes for the container.

In this way property names are always in the pair string and the content/value is in the `_content`.

```c++
Data node;
Data::readFile(node, path);

std::cout << node.getObj(0).first << "\n";
std::cout << node.getObj(0).second.getObj(0).first << "\n";
std::cout << node.getObj(0).second.getObj(0).second.getContent() << "\n";

// but now we have a new way of doing things but it's not detailed here.
...

server
url
https://example.com/

```

Maybe an idea might be to overload the [] for data, so it's shorthand for the .getObj(n). Acess could look like this `node[0].second[0].first`;

server the real config {
	listen = 127.0.0.1:8080
	body_limit = 1048576
	server_name = test www.test.org
	location / {
		alias = basic_site/
		methods = GET POST
		redir = something/dir/here
		dir_listing = true
		default_file = index.html
		cgi = php python
		upload_dir = some/dir/
	}
	location /folder {
		alias = basic_site/folder
	}
	locaiton /fooba/this {

	}
	locaioon /foobar/this/that {

	}
}

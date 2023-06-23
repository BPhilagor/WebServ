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
		save_dir = some/dir/
	}
	location /foobar {
		alias = some_other_site_bad
	}
	locaiton /fooba/this {

	}
	locaioon /foobar/this/that {

	}
}

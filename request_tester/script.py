# needs to be installed with : python3 -m pip install requests
import requests
# import os
import asyncio
import aiohttp

async def fetch(session, url):
	async with session.get(url) as response:
		return response.status

async def main():
	urls = [
		'http://localhost:8080',
		'http://localhost:8080/hello',
		'http://localhost:8080/hello/df',
		'http://localhost:8080/folder',
		'http://localhost:8080/tst',
		'http://localhost:8080/tst',
		'http://localhost:8080/secret',
		'http://localhost:8080/broken',

		'http://localhost:4242/asdas',
		'http://localhost:4242/',
		'http://localhost:4242/foo/bar/',

		'http://localhost:96/',

		'http://localhost:5555/'
	]

	for i in range(0, 1000):
		urls.append('http://localhost:96/')


	async with aiohttp.ClientSession() as session:
		tasks = []
		for url in urls:
			task = asyncio.ensure_future(fetch(session, url))
			tasks.append(task)

		response_codes = await asyncio.gather(*tasks)
		for url, response_code in zip(urls, response_codes):
			print(f"{response_code} : {url}")

loop = asyncio.get_event_loop()

asyncio.run(main())

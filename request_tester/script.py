# needs to be installed with : python3 -m pip install requests
import requests

r = requests.get("https://localhost:8080")

print(r.url)

#!/usr/bin/env python
import random

def generate_html_page(joke):
	html = f'''
	<!DOCTYPE html>
	<html>
	<head>
		<title>Random Joke Generator</title>
		<style>
			body {{
				font-family: Arial, sans-serif;
				text-align: center;
				background-color: #f7f7f7;
			}}

			h1 {{
				color: #333;
			}}

			p {{
				color: #777;
				font-size: 18px;
				margin-bottom: 20px;
			}}

			button {{
				background-color: #4CAF50;
				border: none;
				color: white;
				padding: 10px 20px;
				text-align: center;
				text-decoration: none;
				display: inline-block;
				font-size: 16px;
				margin-top: 20px;
				cursor: pointer;
				border-radius: 5px;
				transition: background-color 0.3s ease;
			}}

			button:hover {{
				background-color: #45a049;
			}}

		.subscript {{
			font-size: 12px;
			color: #999;
			margin-top: 50px;
			text-align: center;
		}}
		</style>
	</head>
	<body>
		<h1>Random Joke Generator</h1>
		<p>{joke}</p>
		<button onclick="window.location.reload();">Tell me another joke!</button>
		<p> </p>
		<p class="subscript">This is used to test a CGI script as the default file for a directory.</p>
	</body>
	</html>
	'''
	return html

jokes = [
	"Why don't scientists trust atoms? Because they make up everything!",
	"Did you hear about the mathematician who's afraid of negative numbers? He will stop at nothing to avoid them!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why did the scarecrow win an award? Because he was outstanding in his field!",
	"Why did the bicycle fall over? It was two-tired!",
	"Why don't eggs tell jokes? Because they might crack up!",
	"What do you call a bear with no teeth? A gummy bear!",
	"Why did the tomato turn red? Because it saw the salad dressing!",
	"What do you call a fish wearing a crown? King Neptune!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why did the golfer bring two pairs of pants? In case he got a hole in one!",
	"Why don't scientists trust atoms? Because they make up everything!",
	"Why did the scarecrow win an award? Because he was outstanding in his field!",
	"What did one wall say to the other wall? I'll meet you at the corner!",
	"Why did the bicycle fall over? It was two-tired!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why don't eggs tell jokes? Because they might crack up!",
	"What do you call a bear with no teeth? A gummy bear!",
	"Why did the tomato turn red? Because it saw the salad dressing!",
	"What do you call a fish wearing a crown? King Neptune!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why did the golfer bring two pairs of pants? In case he got a hole in one!",
	"Why don't scientists trust atoms? Because they make up everything!",
	"Why did the scarecrow win an award? Because he was outstanding in his field!",
	"What did one wall say to the other wall? I'll meet you at the corner!",
	"Why did the bicycle fall over? It was two-tired!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why don't eggs tell jokes? Because they might crack up!",
	"What do you call a bear with no teeth? A gummy bear!",
	"Why did the tomato turn red? Because it saw the salad dressing!",
	"What do you call a fish wearing a crown? King Neptune!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why did the golfer bring two pairs of pants? In case he got a hole in one!",
	"Why don't scientists trust atoms? Because they make up everything!",
	"Why did the scarecrow win an award? Because he was outstanding in his field!",
	"What did one wall say to the other wall? I'll meet you at the corner!",
	"Why did the bicycle fall over? It was two-tired!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why don't eggs tell jokes? Because they might crack up!",
	"What do you call a bear with no teeth? A gummy bear!",
	"Why did the tomato turn red? Because it saw the salad dressing!",
	"What do you call a fish wearing a crown? King Neptune!",
	"Why don't skeletons fight each other? They don't have the guts!",
	"Why did the golfer bring two pairs of, .. whos knows GPT stopped generating"
]

# Select a random joke
random_joke = random.choice(jokes)

# Generate HTML page with the random joke
generated_html = generate_html_page(random_joke)

# Print CGI header and generated HTML
print("Content-Type: text/html")
print()
print(generated_html)

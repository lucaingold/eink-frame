from flask import Flask
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Enable CORS for all routes

# Your existing routes and server setup here

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8080)
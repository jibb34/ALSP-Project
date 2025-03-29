from flask import Flask, jsonify, request

app = Flask(__name__)


@app.route('/')
def index():
    return jsonify({"message": "Welcome to the PXE Web Interface!"})


@app.route('/configure', methods=['POST'])
def configure():
    data = request.json
    # Process your Linux environment configuration here
    return jsonify({"status": "Configuration updated successfully!"})


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000)

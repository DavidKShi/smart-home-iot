from flask import Flask, request, render_template, jsonify

app = Flask(__name__)

# Current state of sensors and LED
data = {
    "temperature": 0,
    "humidity": 0,
    "motion": 0,
    "led": 0
}


@app.route("/")
def dashboard():
    return render_template("dashboard.html")


@app.route("/data", methods=["POST"])
def receive_data():
    global data

    incoming = request.json

    data["temperature"] = incoming.get("temperature", 0)
    data["humidity"] = incoming.get("humidity", 0)
    data["motion"] = incoming.get("motion", 0)

    return "OK"


@app.route("/get_data")
def get_data():
    return jsonify(data)


@app.route("/led", methods=["GET", "POST"])
def led_control():
    global data

    if request.method == "POST":
        cmd = request.json

        data["led"] = cmd.get("led", 0)

        print(f"LED state changed to: {data['led']}")

        return "OK"

    return jsonify({"led": data["led"]})


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, threaded=True)
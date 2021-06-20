extends Control

export(Vector2) var right_input = Vector2()
export(Vector2) var left_input = Vector2()

var pressed:bool = false
var just_pressed:bool = false
var just_released:bool = false

func _on_left_stick_control_signal(speed, angle, vector):
	left_input = vector
	input_received(vector, 0)
	pass

func _on_right_stick_control_signal(speed, angle, vector):
	right_input = vector
	input_received(vector, 1)
	pass

func input_received(vector,id):
	match id:
		0:
			send_stick_event(vector.x, JOY_ANALOG_LX)
			send_stick_event(vector.y, JOY_ANALOG_LY)
		1:
			send_stick_event(vector.x, JOY_ANALOG_RX)
			send_stick_event(vector.y, JOY_ANALOG_RY)
	pass

func send_stick_event(value, id):
	var ev = InputEventJoypadMotion.new()
	ev.set_axis_value(value)
	ev.set_axis(id)
	Input.parse_input_event(ev)
	pass


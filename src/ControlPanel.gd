extends Node2D

onready var control = $CenterContainer/Controller

func _ready():
	pass

func _process(delta):
	var x_r = control.x_g
	var y_r = control.y_g
	var z_r = control.z_g
	$Sprite.rotation_degrees = -y_r
	#$MeshInstance.rotation_degrees.y = z_r
	$Sprite.position.y = x_r*2 + 300
	$LabelTemp.text = str(control.temp) + " C"
	$Throttlebar.value = control.throttle
	$SignalBar.value = control.sStrength
	pass

void initialize_MLX90614() {
	if (!mlx.begin()) {
		Serial.println("Error connecting to MLX sensor. Check wiring.");
		while (1);
	};
}

String readMLX90614() {
	
  return String(mlx.readAmbientTempC()) + String(", ") + String(mlx.readObjectTempC()) + String(", ");

}
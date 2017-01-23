function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {corner:[]};

  // if (port === 1) decoded.led = bytes[0];
  decoded.corner[0] = (bytes[1] << 8) + bytes[0];
  decoded.corner[1] = (bytes[3] << 8) + bytes[2];
  decoded.corner[2] = (bytes[5] << 8) + bytes[4];
  decoded.corner[3] = (bytes[7] << 8) + bytes[6];

  return decoded;
}

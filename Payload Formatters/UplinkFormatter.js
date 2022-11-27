function Decoder(bytes, port) {
    var t = (bytes[0] & 0x80 ? 0xFFFF<<16 : 0) | bytes[0]<<8 | bytes[1];
    var h = bytes[2]<<8 | bytes[3];
    return {
      temperature: t / 100,
      humidity: h / 100
    }
  }
  
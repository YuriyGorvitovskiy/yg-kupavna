#ifndef block_h
#define block_h

enum BlockState {
  BLOCK_DISABLED,
  BLOCK_FREE,
  BLOCK_OCCUPIED,
  BLOCK_SHORT
};

class Block {
public:

  Block(unsigned char power_pin, unsigned char occupy_pin, unsigned char short_pin, void (*onChange)(Block& block));
  void setup();
  void loop();

  BlockState getState();
  bool       isFree();
  bool       isOccupied();

protected:
  const unsigned char m_power_pin;
  const unsigned char m_occupy_pin;
  const unsigned char m_short_pin;
  void              (*m_onChange)(Block& block);

  BlockState          m_state;
};

inline BlockState Block::getState() {
  return m_state;
}

inline boolean Block::isFree() {
  return BlockState::BLOCK_FREE == m_state;
}

inline boolean Block::isOccupied() {
  return BlockState::BLOCK_OCCUPIED == m_state;
}

#endif  //block_h
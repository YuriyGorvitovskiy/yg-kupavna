#include <Arduino.h>
#include "block.h"

Block::Block(unsigned char power_pin, unsigned char occupy_pin, unsigned char short_pin, void (*onChange)(Block& block)) :
  m_power_pin(power_pin),
  m_occupy_pin(occupy_pin),
  m_short_pin(short_pin),
  m_onChange(onChange),
  m_state(BlockState::BLOCK_DISABLED){
}

void Block::setup() {
  pinMode(m_power_pin, INPUT_PULLUP);
  pinMode(m_occupy_pin, INPUT_PULLUP);
  pinMode(m_short_pin, INPUT_PULLUP);
}

void Block::loop() {
  BlockState new_state = m_state;
  if (LOW == digitalRead(m_short_pin)) {
    new_state = BlockState::BLOCK_SHORT;
  } else if (LOW == digitalRead(m_occupy_pin)) {
    new_state = BlockState::BLOCK_OCCUPIED;
  } else if (LOW == digitalRead(m_power_pin)) {
    new_state = BlockState::BLOCK_FREE;
  } else {
    new_state = BlockState::BLOCK_DISABLED;
  }
  if (new_state == m_state) {
    return;
  }
  m_state = new_state;
  if (nullptr != m_onChange) {
    (*m_onChange)(*this);
  }
}
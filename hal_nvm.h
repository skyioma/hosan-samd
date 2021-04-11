#ifndef HAL_NVM_H
#define HAL_NVM_H

#include <stdint.h>

void hal_nvm_init(void);
uint32_t hal_nvm_bsec_state_save(const uint8_t *state_buffer, uint32_t n_buffer);
uint32_t hal_nvm_bsec_state_load(uint8_t *state_buffer, uint32_t n_buffer);
void hal_nvm_bsec_state_erase(void);

#endif

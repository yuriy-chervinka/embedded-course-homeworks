#include "filter.h"
#include <string.h>

void filter_sma_init(filter_sma_t *self) {
    if (self == NULL) return;
    memset(self, 0, sizeof(filter_sma_t));
}

uint32_t filter_sma_update(filter_sma_t *self, uint32_t new_sample) {
    if (self == NULL) return new_sample;

    self->running_sum -= self->buffer[self->buffer_idx];
    self->buffer[self->buffer_idx] = new_sample;
    self->running_sum += new_sample;
    self->buffer_idx = (self->buffer_idx + 1) & FILTER_CIRCULAR_WINDOW_MASK;
    return self->running_sum >> 4;
}

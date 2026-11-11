/*
 * heart_loss.c
 *
 * Created: 11.11.2026 13:55:52
 *  Author: claude.ai
 */ 

// heart_sounds.c
#include <stdint.h>
#include <stddef.h>
#include "da_beep.h"

typedef struct {
	uint16_t freq;    // Hz, 0 = rest
	uint16_t dur_ms;
} da_note_t;

// da_beep_blocking's 2nd argument is 1000 / duration_ms (5 -> 200 ms).
static void beep_ms_once(uint16_t freq, uint16_t ms)
{
	if (ms == 0) return;
	uint16_t v = (uint16_t)((1000u + ms / 2) / ms);   // rounded
	if (v < 1) v = 1;                                 // 1000 ms is the max
	da_beep_blocking(freq, v);
}

// Any duration: long notes are split into 100 ms chunks.
static void beep_ms(uint16_t freq, uint16_t ms)
{
	while (ms >= 150) {
		da_beep_blocking(freq, 10);   // exactly 100 ms
		ms -= 100;
	}
	beep_ms_once(freq, ms);
}

static void da_rest(uint16_t ms)
{
	beep_ms(0, ms);   // if freq 0 misbehaves in your driver, use a delay here instead
}

static void da_play_notes(const da_note_t *notes, size_t count)
{
	for (size_t i = 0; i < count; i++)
	beep_ms(notes[i].freq, notes[i].dur_ms);
}

// 1. Heart crack: a sharp, very short snap
void da_heart_crack(void)
{
	beep_ms(2400, 12);   // value 83
	beep_ms(1200, 15);   // value 67
	beep_ms(300, 50);    // value 20
}

// 2. Heart split: shards falling, pseudo-random pitches drifting down
void da_heart_split(void)
{
	uint32_t seed = 12345;
	uint16_t base = 1800;

	for (int i = 0; i < 14; i++) {
		seed = seed * 1103515245u + 12345u;
		uint16_t jitter = (seed >> 16) % (base / 3 + 1);
		beep_ms(base - jitter, 25);   // value 40
		base = (uint16_t)(base * 85 / 100);
		if (base < 150) base = 150;
	}
}

// 3. Game over music: generic player plus an ORIGINAL placeholder tune
void da_play_song(const da_note_t *song, size_t count)
{
	da_play_notes(song, count);
}

static const da_note_t placeholder_song[] = {
	{330, 400}, {294, 400}, {262, 400}, {247, 600},
	{0, 200},
	{262, 400}, {247, 400}, {220, 800},
};

void da_game_over_music(void)
{
	da_play_song(placeholder_song,
	sizeof placeholder_song / sizeof placeholder_song[0]);
}

// Whole sequence
void da_heart_loss_sequence(void)
{
	da_heart_crack();
	da_rest(1000);
	da_rest(1000);
	da_heart_split();
	da_rest(1000);
	da_game_over_music();
}
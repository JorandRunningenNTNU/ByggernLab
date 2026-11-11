/*
 * heart_loss.h
 *
 * Created: 11.11.2026 14:03:33
 *  Author: claude.ai
 */ 


#ifndef HEART_LOSS_H_
#define HEART_LOSS_H_


#pragma once
#include <stdint.h>
#include <stddef.h>

// One note: frequency in Hz (0 = rest) and duration in milliseconds.
typedef struct {
	uint16_t freq;
	uint16_t dur_ms;
} da_note_t;

// 1. Heart crack: a sharp, very short snap.
void da_heart_crack(void);

// 2. Heart split: shards falling, pitches drifting down.
void da_heart_split(void);

// 3. Game over music: plays the song table in heart_sounds.c.
void da_game_over_music(void);

// Plays any table of notes (blocking).
void da_play_song(const da_note_t *song, size_t count);

// Whole sequence: crack, split, game over music (blocking).
void da_heart_loss_sequence(void);

#endif /* HEART_LOSS_H_ */
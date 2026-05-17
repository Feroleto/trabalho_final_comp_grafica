#pragma once
#include "PlayerAction.h"
#include "../../inputs/input_buffer.h"

struct PlayerInput {
    PlayerAction update(const InputBuffer& buffer) {
        InputFrame last = buffer.getLast();
        PlayerAction actionIntention = PlayerAction::NONE;
        // Exemplo de mapeamento simples, você pode expandir isso para incluir mais ações e combinações
        if (last.buttons & 0x01) { // Supondo que o bit 0 seja o botão de ataque
            actionIntention = PlayerAction::LIGHT_ATTACK;
        }
        if (last.buttons & 0x02) { // Supondo que o bit 1 seja o botão de pulo
            actionIntention = PlayerAction::JUMP;
        } 
        if (last.direction == 0) {
            actionIntention = PlayerAction::NONE;
        }
        if (last.direction == 1) {
            actionIntention = PlayerAction::WALK_DOWN_LEFT;
        }
        if (last.direction == 2) {
            actionIntention = PlayerAction::WALK_DOWN;
        } 
        if (last.direction == 3) {
            actionIntention = PlayerAction::WALK_DOWN_RIGHT;
        }
        if (last.direction == 6) {
            actionIntention = PlayerAction::WALK_RIGHT;
        }
        if (last.direction == 9) {
            actionIntention = PlayerAction::WALK_UP_RIGHT;
        }
        if (last.direction == 8) {
            actionIntention = PlayerAction::WALK_UP;
        }
        if (last.direction == 7) {
            actionIntention = PlayerAction::WALK_UP_LEFT;
        }
        if (last.direction == 4) {
            actionIntention = PlayerAction::WALK_LEFT;
        }

        return actionIntention;

    }

};
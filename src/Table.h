#ifndef TABLE_H
#define TABLE_H

#include <napi.h>

#define POKER_THROW_ON_ERROR
#include "poker/table.hpp"

class Table : public Napi::ObjectWrap<Table> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    Table(const Napi::CallbackInfo& info);

    static Napi::FunctionReference constructor;

private:
    // Observers
    Napi::Value GetSeats(const Napi::CallbackInfo& info);
    Napi::Value GetForcedBets(const Napi::CallbackInfo& info);
    Napi::Value IsHandInProgress(const Napi::CallbackInfo& info);
    Napi::Value IsBettingRoundInProgress(const Napi::CallbackInfo& info);
    Napi::Value AreBettingRoundsCompleted(const Napi::CallbackInfo& info);
    Napi::Value GetHandPlayers(const Napi::CallbackInfo& info);
    Napi::Value GetButton(const Napi::CallbackInfo& info);
    Napi::Value GetPlayerToAct(const Napi::CallbackInfo& info);
    Napi::Value GetNumActivePlayers(const Napi::CallbackInfo& info);
    Napi::Value GetPots(const Napi::CallbackInfo& info);
    Napi::Value GetRoundOfBetting(const Napi::CallbackInfo& info);
    Napi::Value GetCommunityCards(const Napi::CallbackInfo& info);
    Napi::Value GetLegalActions(const Napi::CallbackInfo& info);
    Napi::Value GetAutomaticActions(const Napi::CallbackInfo& info);
    Napi::Value CanSetAutomaticAction(const Napi::CallbackInfo& info);
    Napi::Value GetLegalAutomaticActions(const Napi::CallbackInfo& info);
    Napi::Value GetHoleCards(const Napi::CallbackInfo& info);

    // Modifiers
    void SetForcedBets(const Napi::CallbackInfo& info);
    void SitDown(const Napi::CallbackInfo& info);
    void StandUp(const Napi::CallbackInfo& info);
    void StartHand(const Napi::CallbackInfo& info);
    void ActionTaken(const Napi::CallbackInfo& info);
    void EndBettingRound(const Napi::CallbackInfo& info);
    void Showdown(const Napi::CallbackInfo& info);
    void SetAutomaticAction(const Napi::CallbackInfo& info);

    poker::table _table;
};

#endif // TABLE_H

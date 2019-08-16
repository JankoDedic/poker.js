#include "Table.h"

#include <random>
#include <stdexcept>

Napi::Value ToValue(Napi::Env env, poker::card_rank rank) {
    constexpr const char* rank_strings[] = { "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A" };
    return Napi::String::New(env, rank_strings[static_cast<int>(rank)]);
}

Napi::Value ToValue(Napi::Env env, poker::card_suit suit) {
    /* constexpr const char* suit_strings[] = { u8"\u2663", u8"\u2666", u8"\u2665", u8"\u2660" }; */
    constexpr const char* suit_strings[] = { "clubs", "diamonds", "hearts", "spades" };
    return Napi::String::New(env, suit_strings[static_cast<int>(suit)]);
}

Napi::Value ToValue(Napi::Env env, const poker::card& c) {
    auto obj = Napi::Object::New(env);
    obj.Set("rank", ToValue(env, c.rank));
    obj.Set("suit", ToValue(env, c.suit));
    return obj;
}

Napi::Value ToValue(Napi::Env env, const poker::hole_cards& hc) {
    auto obj = Napi::Object::New(env);
    obj.Set("first", ToValue(env, hc.first));
    obj.Set("second", ToValue(env, hc.second));
    return obj;
}

Napi::Value ToValue(Napi::Env env, const poker::player& p) {
    auto obj = Napi::Object::New(env);
    obj.Set("betSize", Napi::Number::New(env, p.bet_size()));
    obj.Set("totalChips", Napi::Number::New(env, p.total_chips()));
    obj.Set("stackSize", Napi::Number::New(env, p.stack()));
    return obj;
}

Napi::Value ToValue(Napi::Env env, const poker::community_cards& cc) {
    auto cards = cc.cards();
    auto arr = Napi::Array::New(env, cards.size());
    for (int i = 0; i < cards.size(); ++i) {
        arr.Set(i, ToValue(env, cards[i]));
    }
    return arr;
}

Napi::Value ToValue(Napi::Env env, const poker::round_of_betting& rob) {
    constexpr const char* rob_strings[] = { "preflop", "flop", "turn", "river" };
    switch (rob) {
    case poker::round_of_betting::preflop: return Napi::String::New(env, "preflop");
    case poker::round_of_betting::flop:    return Napi::String::New(env, "flop");
    case poker::round_of_betting::turn:    return Napi::String::New(env, "turn");
    case poker::round_of_betting::river:   return Napi::String::New(env, "river");
    default: throw std::invalid_argument{"Internal error: invalid round_of_betting object"};
    }
}

Napi::Value ToValue(Napi::Env env, const poker::table::automatic_action& aa) {
    switch (aa) {
    case poker::table::automatic_action::fold:       return Napi::String::New(env, "fold");
    case poker::table::automatic_action::check_fold: return Napi::String::New(env, "check/fold");
    case poker::table::automatic_action::check:      return Napi::String::New(env, "check");
    case poker::table::automatic_action::call:       return Napi::String::New(env, "call");
    case poker::table::automatic_action::call_any:   return Napi::String::New(env, "call any");
    case poker::table::automatic_action::all_in:     return Napi::String::New(env, "all in");
    default: throw std::invalid_argument{"Internal error: invalid automatic_action object"};
    }
}

Napi::Value ToValue(Napi::Env env, const poker::dealer::action& a) {
    switch (a) {
    case poker::dealer::action::fold:  return Napi::String::New(env, "fold");
    case poker::dealer::action::check: return Napi::String::New(env, "check");
    case poker::dealer::action::call:  return Napi::String::New(env, "call");
    case poker::dealer::action::bet:   return Napi::String::New(env, "bet");
    case poker::dealer::action::raise: return Napi::String::New(env, "raise");
    default: throw std::invalid_argument{"Internal error: invalid dealer::action object"};
    }
}

Napi::Value ToValue(Napi::Env env, const poker::chip_range& cr) {
    auto obj = Napi::Object::New(env);
    obj.Set("min", Napi::Number::New(env, cr.min));
    obj.Set("max", Napi::Number::New(env, cr.max));
    return obj;
}

Napi::Value ToValue(Napi::Env env, const poker::dealer::action_range& ar) {
    using action = poker::dealer::action;

    auto obj = Napi::Object::New(env);
    auto arr = Napi::Array::New(env);
    auto i = 0;
    if (static_cast<bool>(ar.action & action::fold)) {
        arr.Set(i++, ToValue(env, action::fold));
    }
    if (static_cast<bool>(ar.action & action::check)) {
        arr.Set(i++, ToValue(env, action::check));
    }
    if (static_cast<bool>(ar.action & action::call)) {
        arr.Set(i++, ToValue(env, action::call));
    }
    if (static_cast<bool>(ar.action & action::bet)) {
        arr.Set(i++, ToValue(env, action::bet));
        obj.Set("chipRange", ToValue(env, ar.chip_range));
    }
    if (static_cast<bool>(ar.action & action::raise)) {
        arr.Set(i++, ToValue(env, action::raise));
        obj.Set("chipRange", ToValue(env, ar.chip_range));
    }
    obj.Set("actions", arr);
    return obj;
}

Napi::FunctionReference Table::constructor;

Napi::Object Table::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "Table", {
        InstanceMethod("seats", &Table::GetSeats),
        InstanceMethod("forcedBets", &Table::GetForcedBets),
        InstanceMethod("isHandInProgress", &Table::IsHandInProgress),
        InstanceMethod("isBettingRoundInProgress", &Table::IsBettingRoundInProgress),
        InstanceMethod("areBettingRoundsCompleted", &Table::AreBettingRoundsCompleted),
        InstanceMethod("handPlayers", &Table::GetHandPlayers),
        InstanceMethod("button", &Table::GetButton),
        InstanceMethod("playerToAct", &Table::GetPlayerToAct),
        InstanceMethod("numActivePlayers", &Table::GetNumActivePlayers),
        InstanceMethod("pots", &Table::GetPots),
        InstanceMethod("roundOfBetting", &Table::GetRoundOfBetting),
        InstanceMethod("communityCards", &Table::GetCommunityCards),
        InstanceMethod("legalActions", &Table::GetLegalActions),
        InstanceMethod("automaticActions", &Table::GetAutomaticActions),
        InstanceMethod("canSetAutomaticActions", &Table::CanSetAutomaticAction),
        InstanceMethod("legalAutomaticActions", &Table::GetLegalAutomaticActions),
        InstanceMethod("holeCards", &Table::GetHoleCards),

        InstanceMethod("setForcedBets", &Table::SetForcedBets),
        InstanceMethod("sitDown", &Table::SitDown),
        InstanceMethod("standUp", &Table::StandUp),
        InstanceMethod("startHand", &Table::StartHand),
        InstanceMethod("actionTaken", &Table::ActionTaken),
        InstanceMethod("endBettingRound", &Table::EndBettingRound),
        InstanceMethod("showdown", &Table::Showdown),
        InstanceMethod("setAutomaticAction", &Table::SetAutomaticAction)
        });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Table", func);
    return exports;
}

Table::Table(const Napi::CallbackInfo& info) try
    : Napi::ObjectWrap<Table>(info)
{
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    int length = info.Length();

    if (length != 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Expected 1 argument").ThrowAsJavaScriptException();
    }
    auto obj = info[0].As<Napi::Object>();
    const auto ante = obj.Get("ante").As<Napi::Number>();
    const auto small_blind = obj.Get("smallBlind").As<Napi::Number>();
    const auto big_blind = obj.Get("bigBlind").As<Napi::Number>();

    new (&_table) poker::table{poker::forced_bets{poker::blinds{small_blind, big_blind}, ante}};
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetSeats(const Napi::CallbackInfo& info) try {
    auto array = Napi::Array::New(info.Env(), poker::table::num_seats);
    auto seats = _table.seats();
    for (std::size_t i = 0; i < poker::table::num_seats; ++i) {
        if (seats.occupancy()[i]) {
            array.Set(i, ToValue(info.Env(), seats[i]));
        } else {
            array.Set(i, info.Env().Null());
        }
    }
    return array;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetForcedBets(const Napi::CallbackInfo& info) try {
    const auto fb = _table.forced_bets();
    auto obj = Napi::Object::New(info.Env());
    obj.Set("ante", Napi::Number::New(info.Env(), fb.ante));
    obj.Set("smallBlind", Napi::Number::New(info.Env(), fb.blinds.small));
    obj.Set("bigBlind", Napi::Number::New(info.Env(), fb.blinds.big));
    return obj;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::IsHandInProgress(const Napi::CallbackInfo& info) try {
    return Napi::Boolean::New(info.Env(), _table.hand_in_progress());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::IsBettingRoundInProgress(const Napi::CallbackInfo& info) try {
    return Napi::Boolean::New(info.Env(), _table.betting_round_in_progress());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::AreBettingRoundsCompleted(const Napi::CallbackInfo& info) try {
    return Napi::Boolean::New(info.Env(), _table.betting_rounds_completed());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetHandPlayers(const Napi::CallbackInfo& info) try {
    auto array = Napi::Array::New(info.Env(), poker::table::num_seats);
    auto players = _table.hand_players();
    for (std::size_t i = 0; i < poker::table::num_seats; ++i) {
        if (players.filter()[i]) {
            array.Set(i, ToValue(info.Env(), players[i]));
        } else {
            array.Set(i, info.Env().Null());
        }
    }
    return array;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetButton(const Napi::CallbackInfo& info) try {
    return Napi::Number::New(info.Env(), _table.button());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetPlayerToAct(const Napi::CallbackInfo& info) try {
    return Napi::Number::New(info.Env(), _table.player_to_act());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetNumActivePlayers(const Napi::CallbackInfo& info) try {
    return Napi::Number::New(info.Env(), _table.num_active_players());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetPots(const Napi::CallbackInfo& info) try {
    auto pots = _table.pots();
    auto pot_array = Napi::Array::New(info.Env());
    for (auto i = 0; i < pots.size(); ++i) {
        auto object = Napi::Object::New(info.Env());
        object.Set("size", Napi::Number::New(info.Env(), pots[i].size()));
        auto array = Napi::Array::New(info.Env());
        for (auto j = 0; j < pots[i].eligible_players().size(); ++j) {
            array.Set(j, pots[i].eligible_players()[j]);
        }
        object.Set("eligiblePlayers", array);
        pot_array.Set(i, object);
    }
    return pot_array;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetRoundOfBetting(const Napi::CallbackInfo& info) try {
    return ToValue(info.Env(), _table.round_of_betting());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetCommunityCards(const Napi::CallbackInfo& info) try {
    return ToValue(info.Env(), _table.community_cards());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetLegalActions(const Napi::CallbackInfo& info) try {
    return ToValue(info.Env(), _table.legal_actions());
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetAutomaticActions(const Napi::CallbackInfo& info) try {
    auto aa = _table.automatic_actions();
    auto array = Napi::Array::New(info.Env(), poker::table::num_seats);
    for (std::size_t i = 0; i < poker::table::num_seats; ++i) {
        if (aa[i]) {
            array.Set(i, ToValue(info.Env(), *aa[i]));
        } else {
            array.Set(i, info.Env().Null());
        }
    }
    return array;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::CanSetAutomaticAction(const Napi::CallbackInfo& info) try {
    if (info.Length() != 1 && !info[0].IsNumber()) {
        Napi::TypeError::New(info.Env(), "Expected 1 argument of type Number").ThrowAsJavaScriptException();
    }
    const auto seat = info[0].As<Napi::Number>().Int32Value();
    return Napi::Boolean::New(info.Env(), _table.can_set_automatic_action(seat));
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetLegalAutomaticActions(const Napi::CallbackInfo& info) try {
    using automatic_action = poker::table::automatic_action;
    if (info.Length() != 1 && !info[0].IsNumber()) {
        Napi::TypeError::New(info.Env(), "Expected 1 argument of type Number").ThrowAsJavaScriptException();
    }
    auto env = info.Env();
    const auto value = info[0].As<Napi::Number>().Int32Value();
    const auto action = static_cast<automatic_action>(value);
    auto array = Napi::Array::New(info.Env());
    std::size_t i = 0;
    if (static_cast<bool>(action & automatic_action::fold)) {
        array.Set(i++, ToValue(env, automatic_action::fold));
    }
    if (static_cast<bool>(action & automatic_action::check_fold)) {
        array.Set(i++, ToValue(env, automatic_action::check_fold));
    }
    if (static_cast<bool>(action & automatic_action::check)) {
        array.Set(i++, ToValue(env, automatic_action::check));
    }
    if (static_cast<bool>(action & automatic_action::call)) {
        array.Set(i++, ToValue(env, automatic_action::call));
    }
    if (static_cast<bool>(action & automatic_action::call_any)) {
        array.Set(i++, ToValue(env, automatic_action::call_any));
    }
    if (static_cast<bool>(action & automatic_action::all_in)) {
        array.Set(i++, ToValue(env, automatic_action::all_in));
    }
    return array;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

Napi::Value Table::GetHoleCards(const Napi::CallbackInfo& info) try {
    const auto hole_cards = _table.hole_cards();
    auto array = Napi::Array::New(info.Env());
    for (std::size_t i = 0; i < poker::table::num_seats; ++i) {
        if (hole_cards.filter()[i]) {
            array.Set(i, ToValue(info.Env(), hole_cards[i]));
        } else {
            array.Set(i, info.Env().Null());
        }
    }
    return array;
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::SetForcedBets(const Napi::CallbackInfo& info) try {
    if (info.Length() != 1 || !info[0].IsObject()) {
        Napi::TypeError::New(info.Env(), "Expected 1 argument").ThrowAsJavaScriptException();
    }
    auto obj = info[0].As<Napi::Object>();
    const auto ante = obj.Get("ante").As<Napi::Number>();
    const auto small_blind = obj.Get("smallBlind").As<Napi::Number>();
    const auto big_blind = obj.Get("bigBlind").As<Napi::Number>();

    _table.set_forced_bets({{small_blind, big_blind}, ante});
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::SitDown(const Napi::CallbackInfo& info) try {
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(info.Env(), "Expected 2 arguments of type Number").ThrowAsJavaScriptException();
    }
    auto seat = info[0].As<Napi::Number>().Int32Value();
    auto buy_in = info[1].As<Napi::Number>().Int32Value();
    _table.sit_down(seat, buy_in);
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::StandUp(const Napi::CallbackInfo& info) try {
    if (info.Length() != 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(info.Env(), "Expected 1 argument of type Number").ThrowAsJavaScriptException();
    }
    auto seat = info[0].As<Napi::Number>().Int32Value();
    _table.stand_up(seat);
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::StartHand(const Napi::CallbackInfo& info) try {
    static auto rng = std::make_unique<std::mt19937>(std::random_device{}());

    if (info.Length() == 1) {
        if (!info[0].IsNumber()) {
            Napi::TypeError::New(info.Env(), "Invalid argument for button: expected Number").ThrowAsJavaScriptException();
        } else {
            const auto button = info[0].As<Napi::Number>().Int32Value();
            _table.start_hand(*rng, button);
        }
    } else {
        _table.start_hand(*rng);
    }
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::ActionTaken(const Napi::CallbackInfo& info) try {
    static const std::string actions[] = { "fold", "check", "call", "bet", "raise" };
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    }

    auto action = info[0].As<Napi::String>().Utf8Value();
    auto table_action = poker::action{};
    if (action == actions[0]) table_action = poker::action::fold;
    else if (action == actions[1]) table_action = poker::action::check;
    else if (action == actions[2]) table_action = poker::action::call;
    else if (action == actions[3]) table_action = poker::action::bet;
    else if (action == actions[4]) table_action = poker::action::raise;
    else Napi::TypeError::New(info.Env(), "Invalid action").ThrowAsJavaScriptException();

    if (table_action == poker::action::bet || table_action == poker::action::raise) {
        if (info.Length() != 2 || !info[1].IsNumber()) {
            Napi::TypeError::New(info.Env(), "No bet specified").ThrowAsJavaScriptException();
        }
        const auto bet = info[1].As<Napi::Number>().Int32Value();
        _table.action_taken(table_action, bet);
    } else {
        _table.action_taken(table_action);
    }
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::EndBettingRound(const Napi::CallbackInfo& info) try {
    _table.end_betting_round();
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::Showdown(const Napi::CallbackInfo& info) try {
    _table.showdown();
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

void Table::SetAutomaticAction(const Napi::CallbackInfo& info) try {
    static const std::string actions[] = {
        "fold", "check/fold", "check", "call", "call any", "all in"
    };
    if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsString()) {
        Napi::TypeError::New(info.Env(), "Invalid arguments").ThrowAsJavaScriptException();
    }

    const auto seat = info[0].As<Napi::Number>().Int32Value();
    if (seat < 0 || seat > poker::table::num_seats) {
        Napi::TypeError::New(info.Env(), "Invalid seat index").ThrowAsJavaScriptException();
    }

    using automatic_action = poker::table::automatic_action;
    auto action = info[0].As<Napi::String>().Utf8Value();
    auto aa = automatic_action{};
    if (action == actions[0]) aa = automatic_action::fold;
    else if (action == actions[1]) aa = automatic_action::check_fold;
    else if (action == actions[2]) aa = automatic_action::check;
    else if (action == actions[3]) aa = automatic_action::call;
    else if (action == actions[4]) aa = automatic_action::call_any;
    else if (action == actions[5]) aa = automatic_action::all_in;
    else Napi::TypeError::New(info.Env(), "Invalid action").ThrowAsJavaScriptException();

    _table.set_automatic_action(seat, aa);
} catch (const std::exception& e) {
    throw Napi::Error::New(info.Env(), e.what());
}

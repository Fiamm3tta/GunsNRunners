// Fill out your copyright notice in the Description page of Project Settings.


#include "GNRGameplayTags.h"

namespace GNRGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dash, "InputTag.Dash");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Shoot, "InputTag.Shoot");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill, "InputTag.Skill");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Unequip, "InputTag.Unequip");

	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Jump, "Player.Ability.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dash, "Player.Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Shoot, "Player.Ability.Shoot");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill, "Player.Ability.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip, "Player.Ability.Unequip");

	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Dash, "Player.Cooldown.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Shoot, "Player.Cooldown.Shoot");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Skill, "Player.Cooldown.Skill");

	/** Enemy Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Attack, "Enemy.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Charge, "Enemy.Ability.Charge");

	/** Shared Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, "Shared.Status.Death");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_AttackPower, "Shared.SetByCaller.AttackPower");
}

#include "TheRock.h"

TheRock::TheRock()
{
	EM.AddLineModel(PowerUp = DBG_NEW LineModel());
}

TheRock::~TheRock()
{
}

void TheRock::SetPlayer(ThePlayer* player)
{
	Player = player;
}

void TheRock::SetAntiPlayer(TheAntiPlayer* player)
{
	AntiPlayer = player;
}

void TheRock::SetPowerUpModel(std::vector<Vector3> model)
{
	PowerUp->SetModel(model);
}

void TheRock::SetExplodeSound(Sound sound)
{
	ExplodeSound = sound;
	SetSoundVolume(ExplodeSound, 0.2f);
}

bool TheRock::Initialize()
{
	LineModel::Initialize();

	return false;
}

bool TheRock::BeginRun()
{
	LineModel::BeginRun();

	PowerUp->SetParent(*this);

	return false;
}

void TheRock::Update(float deltaTime)
{
	LineModel::Update(deltaTime);

	CheckCollisions();
}

void TheRock::FixedUpdate(float deltaTime)
{
	LineModel::FixedUpdate(deltaTime);

	CheckScreenEdge();
}

void TheRock::Draw3D()
{
	LineModel::Draw3D();
}

void TheRock::Spawn(Vector3 position, int waveNumber, RockSize size)
{
	LineModel::Spawn(position);

	WaveNumber = waveNumber;
	PowerUp->Enabled = false;

	float magnitude = 0;
	float angle = M.GetRandomRadian();
	float maxVS = 0;

	TheRock::Size = size;
	float scale = 1; //Was 25
	float change = 0;

	switch (size)
	{
	case Small:
		change = 4.25f;
		Scale = scale / change;
		maxVS = 3;
		magnitude = M.GetRandomFloat(52.3f, 74.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Medium:
		change = 2.75f;
		Scale =  scale / change;
		maxVS = 2;
		magnitude = M.GetRandomFloat(43.2f, 63.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case MediumLarge:
		change = 1.85f;
		Scale =  scale / change;
		maxVS = 2;
		magnitude = M.GetRandomFloat(36.1f, 56.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);
		break;
	case Large:
		Y(M.GetRandomFloat((float)-WindowHalfHeight, (float)WindowHalfHeight));
		Scale = scale;
		maxVS = 1;

		magnitude = M.GetRandomFloat(20.35f, 40.1f);
		Velocity = GetVelocityFromAngleZ(angle, magnitude);

		if (Velocity.x > 0)
		{
			X((float)-WindowHalfWidth);
		}
		else
		{
			X((float)WindowHalfWidth);
		}

		break;
	}

	float rVel = M.GetRandomFloat(-maxVS, maxVS);

	if (rVel < 0.1)
	{
		rVel++;
	}
	else if (rVel > -0.1)
	{
		rVel--;
	}

	RotationVelocityZ = rVel;

	// Setup PowerUp

	int chance = GetRandomValue(1, 100); //Rework this. Later waves limit Purple on a curve.
	//TraceLog(LOG_INFO, "RandValue: %i", chance);

	PowerUpType = PowerUp::PowerUpType::None;
	PowerUp->ModelColor = GRAY;

	if (WaveNumber < 3)
	{
		if (chance < 5)
		{
			PowerUp->ModelColor = PURPLE;
			PowerUpType = PowerUp::PowerUpType::Purple; //Everything over charged for a time.
		}
		else if (chance < 10)
		{
			PowerUp->ModelColor = BLUE;
			PowerUpType = PowerUp::PowerUpType::Blue; //Shield over charge.
		}
		else if (chance < 20)
		{
			PowerUp->ModelColor = SKYBLUE;
			PowerUpType = PowerUp::PowerUpType::Skyblue; //Gun cooling over charge.
		}
	}
	else
	{
		if (WaveNumber < 5)	chance = GetRandomValue(1, 250);
		else chance = GetRandomValue(1, 500);

		if (chance == 1)
		{
			PowerUp->ModelColor = PURPLE;
			PowerUpType = PowerUp::PowerUpType::Purple; //Everything over charged for a time.
		}
		else if (chance < 6)
		{
			PowerUp->ModelColor = BLUE;
			PowerUpType = PowerUp::PowerUpType::Blue; //Shield over charge.
		}
		else if (chance < 9)
		{
			PowerUp->ModelColor = SKYBLUE;
			PowerUpType = PowerUp::PowerUpType::Skyblue; //Gun cooling over charge.
		}
		else if (chance == 11)
		{
			PowerUp->ModelColor = RED;
			PowerUpType = PowerUp::PowerUpType::Red; //Larger Shots limited number.
		}
		else if (chance > 11 && chance < 15)
		{
			PowerUp->ModelColor = YELLOW;
			PowerUpType = PowerUp::PowerUpType::Yellow; //Two side by side shots limited number.
		}
		else if (chance > 15 && chance < 17)
		{
			PowerUp->ModelColor = ORANGE;
			PowerUpType = PowerUp::PowerUpType::Orange; //Homing Mines limited number.
			// Rocks ignored. (Explodes on impact)
		}
		else if (chance == 17)
		{
			PowerUp->ModelColor = VIOLET;
			PowerUpType = PowerUp::PowerUpType::Violet; //Growing plasma shot limited number.
			// Dies at edge of screen. Annihilates everything in it's path.
		}

	}

	if (PowerUpType != PowerUp::PowerUpType::None)
	{
		PowerUp->Enabled = true;
	}

	return;

	if (WaveNumber > 5)
	{
		if (chance == 3)
		{
			PowerUp->ModelColor = MAGENTA;
			PowerUpType = PowerUp::PowerUpType::Magenta; //Spread Shot limited number.
		}

		if (chance == 4)
		{
			PowerUp->ModelColor = MAROON;
			PowerUpType = PowerUp::PowerUpType::Maroon; //Homing missiles limited number. Ignores rocks. Explodes on impact.
		}
	}

}

void TheRock::Destroy()
{
	Entity::Destroy();

	PowerUp->Destroy();
}

void TheRock::Hit()
{
	Entity::Hit();

	if (!Player->GameOver) PlaySound(ExplodeSound);
}

bool TheRock::CheckCollisions()
{
	float power = 0.92f;

	if (Player->Shield->Enabled &&
		CirclesIntersect(Player->Position, Player->Shield->Radius))
	{
		Player->ShieldHit(Position, Velocity, power);
		return false;
	}

	if (Player->Enabled && !Player->Shield->Enabled && CirclesIntersect(*Player))
	{

		Player->Hit();
		Hit();
		SendScoreToPlayer();

		return true;
	}

	if (AntiPlayer->Shield->Enabled &&
		CirclesIntersect(AntiPlayer->Position, AntiPlayer->Shield->Radius))
	{
		AntiPlayer->ShieldHit(Position, Velocity, power);
		return false;
	}

	for (const auto& shot : Player->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	for (const auto& shot : Player->DoubleShots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	for (const auto& bigShot : Player->BigShots)
	{
		if (bigShot->Enabled && CirclesIntersect(*bigShot))
		{
			bigShot->HitPoints -= 20;

			if (bigShot->HitPoints <= 0) bigShot->Destroy();

			Hit();
			SendScoreToPlayer();
			return true;
		}
	}

	for (const auto& plasmaShot : Player->PlasmaShots)
	{
		if (plasmaShot->Enabled && CirclesIntersect(*plasmaShot))
		{
			Hit();
			SendScoreToPlayer();
			return true;
		}
	}


	for (const auto& shot : AntiPlayer->Shots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			return true;
		}
	}

	for (const auto& shot : AntiPlayer->DoubleShots)
	{
		if (shot->Enabled && CirclesIntersect(*shot))
		{
			shot->Destroy();
			Hit();
			return true;
		}
	}

	for (const auto& bigShot : AntiPlayer->BigShots)
	{
		if (bigShot->Enabled && CirclesIntersect(*bigShot))
		{
			bigShot->HitPoints -= 20;

			if (bigShot->HitPoints <= 0) bigShot->Destroy();

			Hit();
			return true;
		}
	}

	for (const auto& plasmaShot : AntiPlayer->PlasmaShots)
	{
		if (plasmaShot->Enabled && CirclesIntersect(*plasmaShot))
		{
			Hit();
			return true;
		}
	}

	return false;
}

void TheRock::SendScoreToPlayer()
{
	switch (Size)
	{
	case Large:
		Player->ScoreUpdate(20);
		break;
	case Medium:
		Player->ScoreUpdate(50);
		break;
	case Small:
		Player->ScoreUpdate(100);
		break;
	}
}

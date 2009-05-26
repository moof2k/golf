#include "rbeffectsmgr.h"

#include "Rude3DSurface.h"

RBEffectsMgr::RBEffectsMgr(void)
{
	_floor = NULL;
	_particles = NULL;

	Init();
}

RBEffectsMgr::~RBEffectsMgr(void)
{
	Destroy();
}

void RBEffectsMgr::Destroy()
{
	if(_particles)
	{
		for(int i = 0; i < kRBEffectsMaxParticleSystems; i++)
		{
			if(_particles[i] != NULL)
			{
				delete _particles[i];
				_particles[i] = NULL;
			}
		}

		free(_particles);
		_particles = NULL;
	}

	if(_floor)
	{
		delete _floor;
		_floor = NULL;
	}
}

void RBEffectsMgr::Init()
{
	Destroy();

	_floor = new DPlane(0.0, 1.0, 0.0, 0.0);

	_particles = (DParticleSystem **) malloc(sizeof(DParticleSystem *) * kRBEffectsMaxParticleSystems);
	for(int i = 0; i < kRBEffectsMaxParticleSystems; i++)
	{
		_particles[i] = new DParticleSystem();
		_particles[i]->SetLifespan(0.0f);
	}


}


void RBEffectsMgr::Render(RudeSurface &screen, float delta, float aspect, int tilesize)
{

	Rude3DSurface *backbuffer3d = new Rude3DSurface(screen);

	float invtilesize = 1.0f / ((float) tilesize);

	float orthox = (screen.GetWidth() - 1.0f) * invtilesize;
	float orthoy = (screen.GetHeight() - 1.0f) * invtilesize;



	float xo = -5.0f;
	float yo = -10.5f;
	
	float eyex = 0.0f;
	float eyey = 10.5f;

	backbuffer3d->GrlSetViewport(0, 0, screen.GetHeight() - 1, screen.GetWidth() - 1);
	backbuffer3d->GrlOrtho(0.0f, 0.0f, 0.0f, orthox * 0.5f, orthoy * 0.5f, 100.0);
	backbuffer3d->GrlPerspective(10.0f, 100.0f);
	backbuffer3d->GrlLookAt(eyex, eyey, 20.0f, eyex, eyey, 0.0f, 0.0f, 1.0f, 0.0f);
	
	backbuffer3d->GrlLoadIdentity();

	//backbuffer3d->GrlDrawAxis();

	backbuffer3d->GrlTranslateView(xo, 0.0f, 0.0f);

	RudeVertex a, b;
	a.x = 0.0f;
	a.y = 0.0f;
	a.z = 0.0f;

	b.x = 1.0f;
	b.y = 1.0f;
	b.z = 0.0f;

	// backbuffer3d->GrlDraw3DLine(&a, &b, backbuffer3d->GetWhite(), true);


	for(int i = 0; i < kRBEffectsMaxParticleSystems; i++)
	{
		if(_particles[i] != NULL && _particles[i]->_lifetimer != 0.0f)
		{
			_particles[i]->Tick(delta);
			_particles[i]->Collide(_floor);

			_particles[i]->Render(*backbuffer3d);
		}
	}


	delete backbuffer3d;

}


void RBEffectsMgr::NewParticleSystem(int x, int y, int ps, RudeColor *c)
{
	if(_particles[ps] != NULL)
	{
		_particles[ps]->Init((float) x, (float) y, c);
		_particles[ps]->SetLifetimer(0.0001f);
	}

}




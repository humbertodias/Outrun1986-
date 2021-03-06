#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL2_rotozoom.h"

ModuleRender::ModuleRender ()
{
  camera.x = camera.y = 0;
  camera.w = SCREEN_WIDTH * SCREEN_SIZE;
  camera.h = SCREEN_HEIGHT * SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender ()
{
}

// Called before render is available
bool
ModuleRender::Init ()
{
  LOG ("Creating Renderer context");
  bool ret = true;
  Uint32 flags = 0;

  if (VSYNC == true)
    {
      flags |= SDL_RENDERER_PRESENTVSYNC;
    }

  renderer = SDL_CreateRenderer (App->window->window, -1, flags);

  if (renderer == nullptr)
    {
      LOG ("Renderer could not be created! SDL_Error: %s\n", SDL_GetError ());
      ret = false;
    }

  return ret;
}

update_status
ModuleRender::PreUpdate ()
{
  SDL_SetRenderDrawColor (renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear (renderer);
  return UPDATE_CONTINUE;
}

// Called every draw update
update_status
ModuleRender::Update ()
{
  // debug camera
  int speed = 1;

  return UPDATE_CONTINUE;
}

update_status
ModuleRender::PostUpdate ()
{
  SDL_RenderPresent (renderer);
  return UPDATE_CONTINUE;
}

// Called before quitting
bool
ModuleRender::CleanUp ()
{
  LOG ("Destroying renderer");

  //Destroy window
  if (renderer != nullptr)
    {
      SDL_DestroyRenderer (renderer);
    }

  return true;
}

// Blit to screen
bool
ModuleRender::Blit (SDL_Texture * texture, int x, int y, SDL_Rect * section,
		    float speed, int scaledW, int scaledH)
{
  bool ret = true;
  SDL_Rect rect;
  rect.x = (int) (camera.x * speed) + x * SCREEN_SIZE;
  rect.y = (int) (camera.y * speed) + y * SCREEN_SIZE;

  if (section != NULL)
    {
      rect.w = section->w;
      rect.h = section->h;
    }
  else
    {
      SDL_QueryTexture (texture, NULL, NULL, &rect.w, &rect.h);
    }
  if (scaledW != -1 && scaledH != -1)
    {
      rect.w = scaledW;
      rect.h = scaledH;
    }


  rect.w *= SCREEN_SIZE;
  rect.h *= SCREEN_SIZE;

  if (SDL_RenderCopy (renderer, texture, section, &rect) != 0)
    {
      LOG ("Cannot blit to screen. SDL_RenderCopy error: %s",
	   SDL_GetError ());
      ret = false;
    }

  return ret;
}

bool const
ModuleRender::BlitPolygon (Color c, int const x1, int const y1, int const w1,
			   int const x2, int const y2, int const w2)
{
  bool ret = true;
  short s[4] = { x1 - (w1), x1 + (w1), x2 + (w2), x2 - (w2) };
  short t[4] = { y1, y1, y2, y2 };
  filledPolygonRGBA (renderer, s, t, 4, c.r, c.g, c.b, c.a);

  return ret;
}

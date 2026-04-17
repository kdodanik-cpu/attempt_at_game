# Claude Teaching Context — attempt_at_game

## Project Goal
Build a 3D first-person action RPG inspired by Fallout: New Vegas using C++ and Raylib.
Features to implement: gunplay, open world, character build system (skills, traits, perks, items), quest choices.
Scope: solo dev, ambitious but realistic.

## Student Profile
- **Name:** VSP (Krish)
- **C++:** Past beginner. Knows primitive types, pointers, references, classes, structs, public/private, static, extern, std::vector, std::cout, lambdas, iterators. Has been tinkering with Raylib for a few months.
- **Git:** Functional. Knows init, add, commit, push, remote, amend. Still references cheat sheet for anything beyond basics.
- **Learning style:** Task-based, task by task. Give tasks, help when stuck. Explain *why* when it matters, not just *what*.

## Teaching Approach
- Claude decides when a lecture ends and produces the updated context file unprompted.
- Each lecture = one chat session. Paste this file at the start of each new chat.
- No toy examples — every task produces something that becomes part of the actual game.

---

## Lectures Completed

### Lecture 1 — Project Setup & Git Fundamentals
**Concepts covered:**
- Reviewed student's prior work: identified core structural problem (two parallel player representations)
- Why constant refactoring happens: building without a plan for how pieces connect
- `file(GLOB_RECURSE)` in CMake — what it does, when CMake needs to re-run
- `.gitignore` — what to exclude and why
- Binary files in Git — tradeoffs of committing vs ignoring library binaries
- `extern const` — noted as a smell, to be addressed later

**Decisions made:**
- Fresh project, not patching the old one
- Raylib linked locally under `imported_libraries/raylib/`
- Binaries committed (Raylib is small enough, solo dev)
- Folder structure planned: src/ with core/, world/, player/, renderer/, common/

---

### Lecture 2 — Game Loop, Delta Time & First 3D Scene
**Concepts covered:**
- Delta time — why framerate-independent movement is non-negotiable, `velocity * dt` pattern
- `static` at file scope vs in a header
- `extern` as a smell
- Namespace as module — `namespace GameLoop`
- Header minimalism — headers declare, `.cpp` files define
- `main.cpp` should stay thin

**Decisions made:**
- `Player` owns `position` and `velocity` as `Vector3`
- `GameLoop::Update` and `GameLoop::Draw` take `Player&`

---

### Lecture 3 — Mouse-Look, First-Person Camera & Cursor Control
**Concepts covered:**
- Camera ownership — camera belongs to `Player`, not `GameLoop`
- Forward vector math — deriving look direction from yaw/pitch
- Mouse delta is already framerate-independent
- Pitch clamping
- `DisableCursor()` — call once in `Init`
- Dead code in comments is noise

**Decisions made:**
- `Player` owns `yaw`, `pitch`, and `get_camera() const`
- `handle_mouse_movement()` takes no `dt`
- `GameLoop::Init()` added

---

### Lecture 4 — Direction-Relative Movement & Jumping
**Concepts covered:**
- Deriving forward/right vectors from yaw for movement (ignoring pitch)
- Getting forward from camera target-position, zeroing Y, normalizing — valid alternative to raw trig
- Cross product to get right vector from forward + world-up
- Separating vertical (`vertical_speed`) from XZ movement — different systems, don't mix
- `&` vs `&&` — bitwise AND vs logical AND; always `&&` in conditions
- Framerate-dependent gravity — `vertical_speed -= k` runs more times at higher framerates; fix with `* dt`
- Dead code in comments is noise (reinforced)
- Commit discipline — cleanup belongs in the same commit as the work it cleans up

**Decisions made:**
- `Player` has `float vertical_speed` separate from `velocity`
- Gravity applied every frame; hard snap to floor handles overshoot
- Ground was implicit (y=0), replaced in Lecture 5

**Git log at end of lecture:**
```
857dbf6 removed dead code
162a76e player moves according to camera orientation now
8ef999c mouse-look, first-person camera, cursor lock
2ed6766 minor corrections related to game loop
9dac739 game loop, delta time, player stub, first 3d scene
4b6ded8 build system working, blank window
4d58d3a project start
```

---

### Lecture 5 — Basic World Geometry & Vertical Collision
**Concepts covered:**
- `World` as dumb queryable geometry, not a god object
- Composition over hierarchy — `GameLoop` owns instances, entities receive references
- Why `World` should not own or modify `Player` — dependency direction matters; if World takes Player&, it depends on Player, which breaks when you add enemies, NPCs, etc.
- AABB collision via `get_world_height(x, z, y)` — query pattern
- Why `y` is required in the query — filters out boxes whose bottom is above the player, preventing teleportation to undersides
- Gravity always applies every frame; hard snap (`position.y = floor_height`) handles any overshoot regardless of magnitude
- Tolerance/skin width — `+ TOLERANCE` catches player before floor (wrong); `- TOLERANCE` lets small overshoots through (wrong); hard snap is correct
- Sentinel value (`-900.0f`) in candidate_heights keeps container non-empty for `std::max_element` — document this, don't "clean it up"
- `const&` in range-for loops
- Remote repo setup — `git remote add origin`, `git push -u origin main`

**Decisions made:**
- `World::get_world_height(float x, float z, float y) const`
- `Player::update(float dt, const World& world)`
- Floor is a `Box` in `Boxes`, not a special case in `Draw()`
- `static World world` lives in `GameLoop.cpp`

**Current state:**
- Floor and static boxes render and collide correctly
- Player lands on top of boxes
- No horizontal collision
- Project fully pushed to remote GitHub repo

**Git log at end of lecture:**
```
4607ba4 Implemented basic vertical collision
857dbf6 removed dead code
162a76e player moves according to camera orientation now
8ef999c mouse-look, first-person camera, cursor lock
2ed6766 minor corrections related to game loop
9dac739 game loop, delta time, player stub, first 3d scene
4b6ded8 build system working, blank window
4d58d3a project start
```

---

### Lecture 6 — Hitscan Gunplay
**Concepts covered:**
- Raycasting — ray as origin + direction, `GetRayCollisionBox` returns `RayCollision` (hit, distance, point)
- `Gun` struct on `Player` — fire rate, accumulator, `can_fire()`, `on_fired()`, `update(dt)`
- `std::min_element` with a lambda comparator to find closest hit among all candidate collisions
- Lambdas — `[]` capture list, inline anonymous functions, passing as comparator arguments to STL algorithms
- Iterators vs pointers — iterators behave like pointers (`*`, `->`), but `end()` is past-the-end and invalid to dereference
- Always check `iterator != container.end()` before dereferencing
- Unguarded code after a single-line `if` — missing braces causes subsequent lines to run unconditionally regardless of condition
- Uninitialized fields cause undefined behavior from frame one — always initialize in struct definition
- 2D HUD drawing — anything after `EndMode3D()` and before `EndDrawing()` is screen space (pixels, not world coordinates)
- `IsKeyPressed` vs `IsMouseButtonPressed` — wrong function compiles silently, produces wrong behavior at runtime
- Passing `&&` result as function argument — `MOUSE_BUTTON_LEFT && !gun.can_fire()` evaluates to 0 or 1, compiles fine, does the wrong thing
- Integer vs float in `DrawLine` — prefer `GetScreenWidth() / 2` over `* 0.5f`
- GitHub PAT authentication — passwords not accepted for Git operations; classic token with `repo` scope required
- `git commit --amend` — rewrites last commit message or contents; only safe before pushing

**Decisions made:**
- `Player` owns `Gun gun`, `Vector3 last_hit_point = {0}`, `float hit_marker_timer = 0.0f`
- `shoot(const World&)` finds closest hit via `std::min_element`, sets marker fields only inside the `if (closest != end())` guard
- `hit_marker_timer` ticked down in `Player::update` via `std::max(hit_marker_timer - dt, 0.0f)`
- Hit marker drawn as red sphere (`DrawSphere`, radius 0.1f) in `GameLoop::draw` when `hit_marker_timer > 0`
- Crosshair drawn as two `DrawLine` calls after `EndMode3D()`, using `GetScreenWidth() / 2` and `GetScreenHeight() / 2`

**Git log at end of lecture:**
```
(amended) Added crosshair
Transitioned to linux and added a working hitscan system for guns
4607ba4 Implemented basic vertical collision
7702af5 fix gravity framerate dependence, fix bitwise AND bug
857dbf6 removed dead code
162a76e player moves according to camera orientation now
8ef999c mouse-look, first-person camera, cursor lock
2ed6766 minor corrections related to game loop
9dac739 game loop, delta time, player stub, first 3d scene
4b6ded8 build system working, blank window
4d58d3a project start
```

---

## Lecture 7 — (not yet started)
**Candidates based on trajectory:**
- Enemy/NPC stubs — something to shoot at that reacts to being hit
- Damage system — health, hit response
- Horizontal collision — still deferred, may become painful enough to prioritize

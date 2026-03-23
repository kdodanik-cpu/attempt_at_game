git add -A
git commit -m "fix gravity framerate dependence, fix bitwise AND bug"
```

---

## Lecture 5 Preview

Right now your world is a void with a grid floor (or nothing at all). Before we can do quests, guns, or RPG systems, you need something to exist *in*.

Next lecture: **basic world geometry** — loading or constructing a simple level so you have walls to walk around, a ground plane that actually stops you, and a foundation the rest of the game can be built on.

Specifically: a `World` struct, a static collision floor you fall onto properly, and probably a few boxes to navigate around. Nothing fancy — functional scaffolding.

---

Here's the updated context file for your next session:

---
```
# Claude Teaching Context — attempt_at_game
## Project Goal
Build a 3D first-person action RPG inspired by Fallout: New Vegas using C++ and Raylib.
Features to implement: gunplay, open world, character build system (skills, traits, perks, items), quest choices.
Scope: solo dev, ambitious but realistic.
## Student Profile
- **Name:** VSP (Krish)
- **C++:** Past beginner. Knows primitive types, pointers, references, classes, structs, public/private, static, extern, std::vector, std::cout. Has been tinkering with Raylib for a few months.
- **Git:** Functional. Knows init, add, commit, push. Still references cheat sheet for anything beyond basics.
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

### Lecture 3 — Mouse-Look, First-Person Camera & Cursor Control
**Concepts covered:**
- Camera ownership — camera belongs to `Player`, not `GameLoop`
- Forward vector math — deriving look direction from yaw/pitch
- Mouse delta is already framerate-independent
- Pitch clamping
- `DisableCursor()` — call once in `Init`
- Dead code in comments is noise
**Decisions made:**
- `Player` owns `yaw`, `pitch`, and `GetCamera() const`
- `HandleMouseMovement()` takes no `dt`
- `GameLoop::Init()` added

### Lecture 4 — Direction-Relative Movement & Jumping
**Concepts covered:**
- Deriving forward/right vectors from yaw for movement (ignoring pitch)
- Getting forward from camera target-position, zeroing Y, normalizing — valid alternative to raw trig
- Cross product to get right vector from forward + world-up
- Separating vertical (`verticalSpeed`) from XZ movement — different systems, don't mix
- `&` vs `&&` — bitwise AND vs logical AND; always `&&` in conditions
- Framerate-dependent gravity — `verticalSpeed -= k` runs more times at higher framerates; fix with `* dt`
- Dead code in comments is noise (reinforced)
- Commit discipline — cleanup belongs in the same commit as the work it cleans up
**Decisions made:**
- `Player` has `float verticalSpeed` separate from `velocity`
- Gravity only applied when `position.y > 0` (simple ground plane at y=0)
- Ground clamp: when `position.y <= 0 && verticalSpeed < 0`, zero both
**Current state:**
- First-person mouse-look, WASD movement relative to facing direction, jumping with gravity
- Ground is implicit (y=0), no geometry yet
- No collision except the ground clamp

**Git log:**
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
## Lecture 5 — (not yet started)
Planned: Basic world geometry — a `World` struct, static collision floor, some boxes to navigate around. Foundation for everything else.
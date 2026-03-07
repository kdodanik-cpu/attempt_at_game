# Claude Teaching Context — attempt_at_game
## Project Goal
Build a 3D first-person action RPG inspired by Fallout: New Vegas using C++ and Raylib.
Features to implement: gunplay, open world, character build system (skills, traits, perks, items), quest choices.
Scope: solo dev, ambitious but realistic.
## Student Profile
- **Name:** VSP (Krish)
- **C++:** Past beginner. Knows primitive types, pointers, references, classes, structs, public/private, static, extern, std::vector, std::cout. Has been tinkering with Raylib for a few months.
- **Git:** Functional. Knows init, add, commit, push. Still references cheat sheet for anything beyond basics.
- **Learning style:** Task-based. Give tasks, help when stuck. Explain *why* when it matters, not just *what*.
## Teaching Approach
- Claude decides when a lecture ends and produces the updated context file unprompted.
- Each lecture = one chat session. Paste this file at the start of each new chat.
- No toy examples — every task produces something that becomes part of the actual game.
---
## Lectures Completed
### Lecture 1 — Project Setup & Git Fundamentals
**Concepts covered:**
- Reviewed student's prior work: identified core structural problem (two parallel player representations: `Object player` and `Player player1`)
- Why constant refactoring happens: building without a plan for how pieces connect
- `file(GLOB_RECURSE)` in CMake — what it does, when CMake needs to re-run
- `.gitignore` — what to exclude (CLion files, build output, binaries) and why
- Binary files in Git — tradeoffs of committing vs ignoring library binaries
- `extern const` — noted as a smell, to be addressed later

**Decisions made:**
- Fresh project, not patching the old one
- Raylib linked locally under `imported_libraries/raylib/`
- Binaries committed (Raylib is small enough, solo dev)
- Folder structure planned:
  ```
  src/
  ├── main.cpp
  ├── core/
  ├── world/
  ├── player/
  ├── renderer/
  └── common/
  ```

**Git log:**
```
4b6ded8 build system working, blank window
4d58d3a project start
```

---

### Lecture 2 — Game Loop, Delta Time & First 3D Scene
**Concepts covered:**
- Delta time — why framerate-independent movement is non-negotiable, `velocity * dt` pattern
- `static` at file scope vs in a header — one copy per translation unit vs one copy total; why definitions belong in `.cpp` not `.h`
- `extern` as a smell — revisited from Lecture 1, encountered organically when student used `extern Camera camera` across files
- Namespace as module — `namespace GameLoop` vs a singleton class; namespaces for things that are singular
- Header minimalism — headers declare, `.cpp` files define; the header should have no idea the camera exists
- `main.cpp` should stay thin — it bootstraps and runs the loop, it doesn't own data

**Decisions made:**
- `Camera` lives as a `static` in `GameLoop.cpp` — private to that file, will move to `Player` later
- `Player` owns `position` and `velocity` as `Vector3`, updates itself given `dt`
- `GameLoop::Update` and `GameLoop::Draw` take `Player&` — no globals leaking out
- `GetDeltaTime()` removed — it was dead code

**Current folder structure:**
```
src/
├── main.cpp
├── core/
│   ├── GameLoop.h
│   └── GameLoop.cpp
├── player/
│   ├── Player.h
│   └── Player.cpp
├── world/         (empty)
├── renderer/      (empty)
└── common/        (empty)
```

**Git log:**
```
2ed6766 minor corrections related to game loop
9dac739 game loop, delta time, player stub, first 3d scene
4b6ded8 build system working, blank window
4d58d3a project start
```

**State of the project:**
- 3D scene rendering: grey ground plane, purple upright capsule as player placeholder
- WASD movement working in XZ plane with delta time
- First-person camera follows player at eye height, no mouse-look yet

---

## Lecture 3 — (not yet started)
Planned: Mouse-look, proper first-person camera, locking/unlocking cursor.

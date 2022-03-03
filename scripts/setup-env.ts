import { copy, emptyDir } from "https://deno.land/std/fs/mod.ts";

await Deno.stat("main.cpp").catch(() => {
  console.error("Must run script from project root!");
  Deno.exit(-1);
});

await emptyDir("temp");

Deno.chdir("./temp");

// ##################################### ENTT START ###############################################

const clone_entt = [
  "git",
  "clone",
  "--depth",
  "1",
  "-b",
  "v3.9.0",
  "https://github.com/skypjack/entt.git"
];

console.log(clone_entt.join(" "));
await (Deno.run({ cmd: clone_entt })).status();

await emptyDir("../lib");

await copy("./entt/src/entt", "../lib/entt", { overwrite: true });

// ##################################### ENTT END ###############################################

// ##################################### SDL START ###############################################

const clone_sdl = [
  "git",
  "clone",
  "--depth",
  "1",
  "-b",
  "release-2.0.20",
  "https://github.com/libsdl-org/SDL"
];

console.log(clone_sdl.join(" "));
await (Deno.run({ cmd: clone_entt })).status();

Deno.chdir("SDL");
await emptyDir("build");
Deno.chdir("build");

// ############### SDL BUILD ###################

const configure_sdl = [
  "../configure",
];

console.log(configure_sdl.join(" "));
await (Deno.run({ cmd: configure_sdl })).status();

// ##################################### SDL END ###############################################

// Deno.chdir("..");
// await Deno.remove("temp", { recursive: true });

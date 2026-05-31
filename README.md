its pronounced 
KEHL- UI

to all my russians and ukrainains yes im going to translate into russian
my team is english so it would be stupid to use a languge they dont understand

A bloat free graphical user interface 

Hooks the game via custom patchers

that doesn’t use dobby or BNM only openXR


documentation(not finished)
https://keluidocs.vercel.app/#overview

this requires OpenXR nothing else not even dobby 
for mods ofc it can support dobby my main project works with BNM and dobby
for updates every frame with unity BNM is reccomended

how to set this up
make a project
add cmakes
include cpp
add openXR
intergrate youe controller stuff
inisde native lib create your controller configuration with OpenXR
hook openXRlib
hooking the xrlib us very easy read patchinline.cpp that will give you basic understanding



vulkan will render the texture
(this works best paired with wrappers ex BynameModding etc)


what is this
just a floating panel (like how an apk would look)
Hooks openxrloader
with openxrSDK 


# ONLY one LIBARY NEEDED FOR THIS
OpenXR 


```cpp
auto* api = kel::getapi();
Widgets::setcursor(50.0f, 50.0f);

if (api->button("Press me")) {
    // Button press handling would go here.
}

bool enabled = true;
api->checkbox("Enable feature", &enabled);

int quality = 3;
api->sliderint("Quality", &quality, 1, 5);

float brightness = 0.5f;
api->sliderfloat("Brightness", &brightness, 0.0f, 1.0f);
```

### Important note on input

The current implementation of `Widgets::button()` is visual only.
It draws a button and returns `false` always, so actual button click handling is not implemented yet.



1. `init_kelui()` patches `xrCreateSession`, `xrCreateSwapchain`, `xrEnumerateSwapchainImagesVulkanKHR`, `xrEndFrame`, and `vkBeginCommandBuffer`.
2. `h_cs()` captures the Vulkan session bindings from `XrGraphicsBindingVulkanKHR`.
3. `h_csc()` initializes the UI backend when a swapchain is created.
4. `h_esi()` caches swapchain images and builds Vulkan image views for them.
5. `h_ef()` draws the overlay UI for each projection view in `xrEndFrame`.



The current code draws a dark panel and a checkbox:

```cpp
api->rect(50, 50, 350, 500, 0.05f, 0.05f, 0.05f, 0.95f);
api->text(70, 70, "KELUI", 1, 1, 1, 1);
static bool mod1 = true;
api->checkbox("Auto-Aim", &mod1);
```

Then it calls the Vulkan render pass:

```cpp
api->rendervulkan(cmd, e, d.imgs[idx], d.views[idx]);
```



## Source files to edit

- `externalstuff/open.cpp` — main hook and render flow
- `api/kelapi.cpp` — link UI API to backend
- `kel/ui/Widgets.cpp` — widget visuals and layout behavior
- `kel/ui/VulkanBackend.cpp` — Vulkan render pass and command buffer handling

## Practical example

A minimal `xrEndFrame` overlay block can look like this:

```cpp
api->rect(40, 40, 360, 520, 0.08f, 0.08f, 0.1f, 0.98f);
api->text(60, 60, "KelUI Demo", 1.0f, 1.0f, 1.0f, 1.0f);
Widgets::setcursor(60, 100);

bool option = true;
api->checkbox("Option A", &option);
int value = 2;
api->sliderint("Level", &value, 0, 10);

if (api->button("Apply")) {
    // React to button press once input is added.
}
```

## What is not yet implemented

- real pointer/controller input handling
- actual button click detection
- event-driven widget callbacks
- focus navigation for VR controllers



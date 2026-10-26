# Pipeline3d

## Présentation

Pipeline3d est un renderer 3D réalisé en C++.

Le projet utilise Vulkan pour afficher une scène 3D.

Le programme affiche actuellement un cube.

## Prérequis

- Windows x64
- Visual Studio
- Vulkan SDK
- GLFW
- Carte graphique compatible Vulkan

## Installation

Installer le Vulkan SDK et les pilotes graphiques nécessaires.

Récupérer le projet depuis le dépôt Git.

Ouvrir la solution avec Visual Studio.

## Compilation

ouvrir le dossier release puis lancer le programme Pipeline 3d.exe

## Shaders

Les shaders sont compilés en fichiers SPIR-V.

Les fichiers utilisés par le programme sont :

triangle.vert.spv
triangle.frag.spv

## Architecture du code

Le projet est séparé en plusieurs classes.
Chaque classe possède un rôle précis.

### Application

Gère le lancement du programme.

- Initialise le renderer.
- Lance la boucle principale.
- Ferme correctement le programme.

### VulkanContext

Gère la base de Vulkan.

- Crée l'instance Vulkan.
- Crée la surface de la fenêtre.
- Sélectionne le GPU.
- Crée le device.
- Gère les files d'exécution.
- On demande ici a vulkan de quoi il a besoin pour fonctionner.

### SwapChain

Gère les images affichées à l'écran.

- Crée la SwapChain.
- Récupère les images disponibles.
- Crée les Image Views.

### RenderPass

Prépare les étapes du rendu.

Il définit comment les images sont utilisées pendant le rendu.

### GraphicsPipeline

Crée le pipeline utilisé pour dessiner.

Il utilise les shaders et définit les informations nécessaires au rendu du cube.

### Buffer

Gère la création des buffers Vulkan et leur mémoire.

### VertexBuffer

Stocke les données des sommets du cube.

### IndexBuffer

Stocke les indices utilisés pour construire les faces du cube.

### UniformBuffer

Stocke les informations utilisées pour transformer le cube.

Il contient :

- `model`
- `view`
- `projection`

### DescriptorManager

Relie le `UniformBuffer` aux shaders.

Il permet aux shaders d'utiliser les données de transformation.

### CommandManager

Prépare les commandes de rendu.

Il indique notamment :

- Quel pipeline utiliser.
- Quel buffer utiliser.
- Quel objet dessiner.
- Quand effectuer le rendu.

### FrameBuffer

Associe les images de la SwapChain au rendu.

### SyncObject

Gère la synchronisation entre les différentes étapes du rendu.

### Renderer

Coordonne les différentes parties du renderer.

Il permet d'utiliser les ressources Vulkan pour afficher le cube.

---

## Organisation générale

Le fonctionnement du programme suit globalement cet ordre :

```text
Application
    │
    ▼
VulkanContext
    │
    ├── GPU
    ├── Device
    └── Surface
    │
    ▼
SwapChain
    │
    ▼
RenderPass
    │
    ▼
GraphicsPipeline
    │
    ├── Shaders
    └── DescriptorManager
            │
            ▼
       UniformBuffer
    │
    ▼
VertexBuffer + IndexBuffer
    │
    ▼
CommandManager
    │
    ▼
Renderer
    │
    ▼
Cube affiché à l'écran
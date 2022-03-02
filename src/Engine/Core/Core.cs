using Leopotam.EcsLite;

namespace RockyEngine;

public static class Core
{

    public static string Title { get; set; } = "Rocky.NET";

    public static EcsWorld? world;
    public static EcsSystems? systems;

    public static bool Running
    {
        get
        {
            if (Renderer.WindowClosing())
            { running = false; }
            return running;
        }
        set { if (!value) { running = false; } }
    }
    private static bool running = false;
    public static void Initialize(IEnumerable<IEcsSystem>? UserSystems = null)
    {
        world = new EcsWorld();
        systems = new EcsSystems(world);

        systems.Add(new CoreSystem());
        systems.Add(new GraphicsSystem());
        if(UserSystems != null)
        {
            foreach(IEcsSystem system in UserSystems)
            {
                systems.Add(system);
            }
        }
        systems.Init();

        Renderer.Initialize();
        running = true;
    }

    public static void Update()
    {
        systems?.Run();
    }

    public static void Cleanup()
    {
        if (systems != null)
        {
            systems.Destroy();
            systems = null;
        }
        if (world != null)
        {
            world.Destroy();
            world = null;
        }

        Renderer.Cleanup();
        running = false;
    }

    public static int NewEntity()
    {
        return world.NewEntity();
    }
}

public class CoreSystem : IEcsSystem, IEcsRunSystem
{
    public struct CGlobalTransforms
    {
        public Vector3 Position;
        public float Rotation;
        public Vector3 Axis;
        public Vector3 Scale;
    }
    public void Run(EcsSystems systems)
    {
        // Will be called on each EcsSystems.Run() call.

        EcsWorld world = systems.GetWorld();
        // We want to get entities with "CGlobalTransforms".
        var filter = world.Filter<CGlobalTransforms>().End();

        // We want to get pool of "CGlobalTransforms" components.
        var transforms = world.GetPool<CGlobalTransforms>();

        foreach (int entity in filter)
        {
            ref CGlobalTransforms transform = ref transforms.Get(entity);
            transform.Rotation += 1.0f;
        }
    }
}
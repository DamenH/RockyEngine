using RockyEngine;
namespace Game;

public static class Game
{
    public static unsafe void Main()
    {
		Core.Initialize();

		Random RNG = new();
        for (int i = 0; i < 100_000; i++)
        {
            int entity = Core.NewEntity();
			/// TODO: Add methods to Core for adding, removing, and accessing
            Leopotam.EcsLite.EcsPool<CoreSystem.CGlobalTransforms> pool = Core.world.GetPool<CoreSystem.CGlobalTransforms>();
            ref CoreSystem.CGlobalTransforms transform = ref pool.Add(entity);
            
            transform.Position = new Vector3((float)(1000.0 * (RNG.NextDouble() - RNG.NextDouble())),
                                             (float)(10.0 * (RNG.NextDouble() - RNG.NextDouble())),
                                             (float)(1000.0 * (RNG.NextDouble() - RNG.NextDouble())));
            transform.Axis = new Vector3((float)(RNG.NextDouble() - RNG.NextDouble()),
                                         (float)(RNG.NextDouble() - RNG.NextDouble()),
                                         (float)(RNG.NextDouble() - RNG.NextDouble()));
            transform.Scale = new Vector3((float)(1.0 * (0.5 + RNG.NextDouble())),
                                          (float)(1.0 * (0.5 + RNG.NextDouble())),
                                          (float)(1.0 * (0.5 + RNG.NextDouble())));
        }

		while (Core.Running)
		{
			Core.Update();
		}

		Core.Cleanup();
    }
}
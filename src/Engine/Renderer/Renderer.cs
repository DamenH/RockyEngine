#region Usings
global using System.Numerics;
global using Raylib_CsLo;
global using Raylib_CsLo.InternalHelpers;

global using static Raylib_CsLo.Raylib;
global using static Raylib_CsLo.RayMath;
global using static Raylib_CsLo.RayGui;
global using static Raylib_CsLo.RlGl;

global using static Raylib_CsLo.KeyboardKey;
global using static Raylib_CsLo.MouseButton;
global using static Raylib_CsLo.GamepadAxis;
global using static Raylib_CsLo.GamepadButton;
global using static Raylib_CsLo.Gesture;
global using static Raylib_CsLo.ConfigFlags;
global using static Raylib_CsLo.CameraProjection;
global using static Raylib_CsLo.CameraMode;
global using static Raylib_CsLo.TextureFilter;
global using static Raylib_CsLo.TextureWrap;
global using static Raylib_CsLo.ShaderUniformDataType;
global using static Raylib_CsLo.TraceLogLevel;
global using static Raylib_CsLo.MaterialMapIndex;
global using static Raylib_CsLo.CubemapLayout;
global using static Raylib_CsLo.PixelFormat;

global using static Raylib_CsLo.rlFramebufferAttachType;
global using static Raylib_CsLo.rlFramebufferAttachTextureType;
global using static Raylib_CsLo.rlShaderLocationIndex;


global using static Raylib_CsLo.ShaderAttributeDataType;
global using static Raylib_CsLo.ShaderLocationIndex;


//global using static Raylib_CsLo.Examples.RLights;
//global using static Raylib_CsLo.Examples.RLights.LightType;


global using static Raylib_CsLo.BlendMode;
global using static Raylib_CsLo.NPatchLayout;


global using static Raylib_CsLo.FontType;
global using static Raylib_CsLo.MouseCursor;

global using static Raylib_CsLo.Easings;
global using static Raylib_CsLo.Physac;

global using Camera = Raylib_CsLo.Camera3D;
global using RenderTexture2D = Raylib_CsLo.RenderTexture;
global using Texture2D = Raylib_CsLo.Texture;
global using TextureCubemap = Raylib_CsLo.Texture;
global using Matrix = System.Numerics.Matrix4x4;

using Leopotam.EcsLite;
#endregion

namespace RockyEngine;

public static class Renderer
{
    private const int GLSL_VERSION = 330;
    private const int screenWidth = 1024;
    private const int screenHeight = 768;

    static Vector3 position = new(0.0f, 0.0f, 0.0f);
    static float angle = 0;
    public static Model[] model;
    public static Texture2D[] texture;
    static Shader shader;
    public static Camera camera;

    public static unsafe void Initialize()
    {
        SetConfigFlags(FLAG_MSAA_4X_HINT |
                       FLAG_VSYNC_HINT |
                       FLAG_WINDOW_RESIZABLE
                       //FLAG_FULLSCREEN_MODE
                       );
        InitWindow(screenWidth, screenHeight, Core.Title);

        camera = new();
        camera.position = new Vector3(4.0f, 4.0f, 4.0f);
        camera.target = new Vector3(5.0f, 3.0f, 5.0f);
        camera.up = new Vector3(0.0f, 1.0f, 0.0f);
        camera.fovy = 90.0f;
        camera.projection_ = CAMERA_PERSPECTIVE;

        model = new Model[6];
        model[0] = LoadModel("media/Asteroid/asteroid.obj");
        model[1] = LoadModel("media/Asteroid/asteroid_1.obj");
        model[2] = LoadModel("media/Asteroid/asteroid_2.obj");
        model[3] = LoadModel("media/Asteroid/asteroid_3.obj");
        model[4] = LoadModel("media/Asteroid/asteroid_4.obj");
        model[5] = LoadModel("media/Asteroid/asteroid_5.obj");

        texture = new Texture2D[4];
        texture[0] = LoadTexture("media/Asteroid/rock_wall_02_diff_4k.png");
        texture[1] = LoadTexture("media/Asteroid/rock_wall_02_diff_2k.png");
        texture[2] = LoadTexture("media/Asteroid/rock_wall_02_diff_1k.png");
        texture[3] = LoadTexture("media/Asteroid/rock_wall_02_diff.png");
        shader = LoadShader(null, TextFormat("media/shaders/base.fs", GLSL_VERSION));

        foreach (Model m in model)
        {
            m.materials[0].shader = shader;
        }
        model[0].materials[0].maps[(int)MATERIAL_MAP_DIFFUSE].texture = texture[0];
        model[1].materials[0].maps[(int)MATERIAL_MAP_DIFFUSE].texture = texture[1];
        model[2].materials[0].maps[(int)MATERIAL_MAP_DIFFUSE].texture = texture[2];
        model[3].materials[0].maps[(int)MATERIAL_MAP_DIFFUSE].texture = texture[2];
        model[4].materials[0].maps[(int)MATERIAL_MAP_DIFFUSE].texture = texture[3];
        model[5].materials[0].maps[(int)MATERIAL_MAP_DIFFUSE].texture = texture[3];



        SetCameraMode(camera, CAMERA_FIRST_PERSON);

        SetTargetFPS(144);
    }

    public static unsafe void RenderFrame()
    {
        fixed (Camera3D* cam = &camera)
        {
            UpdateCamera(cam);
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        //DrawModel(model, position, 0.5f, WHITE);   // Draw 3d model with texture
        //DrawModelEx(model, position, new Vector3(0.0f, 1.0f, 0.0f), angle, new Vector3(0.5f, 0.5f, 0.5f), WHITE);

        DrawGrid(100, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();
    }

    public static void Cleanup()
    {
        UnloadShader(shader);
        //UnloadTexture(texture);
        //UnloadModel(model);

        CloseWindow();
    }

    public static bool WindowClosing()
    {
        return WindowShouldClose();
    }

    public static Tuple<Vector3, float> QuatToAxis(Quaternion Q)
    {
        // Quat to Axis-Angle
        // angle = 2 * acos(qw)
        // x = qx / sqrt(1-qw*qw)
        // y = qy / sqrt(1-qw*qw)
        // z = qz / sqrt(1-qw*qw)

        // Axis-Angle to Quat
        // q = cos(angle/2) + i ( x * sin(angle/2)) + j (y * sin(angle/2)) + k ( z * sin(angle/2))
        float angle = 2 * MathF.Acos(Q.W);
        float divisor = MathF.Sqrt((float)(1.0 - (Q.W * Q.W)));
        Vector3 axis = new Vector3(
            Q.X / divisor,
            Q.Y / divisor,
            Q.Z / divisor
        );

        return new Tuple<Vector3, float>(axis, angle);
    }
}

public class GraphicsSystem : IEcsSystem, IEcsRunSystem
{
    private static float lodScale = 1.0f;
    public unsafe void Run(EcsSystems systems)
    {
        // Will be called on each EcsSystems.Run() call.

        EcsWorld world = systems.GetWorld();
        // We want to get entities with "CGlobalTransforms".
        var filter = world.Filter<CoreSystem.CGlobalTransforms>().End();
        // We want to get pool of "CGlobalTransforms" components.
        var transforms = world.GetPool<CoreSystem.CGlobalTransforms>();



        fixed (Camera3D* cam = &Renderer.camera)
        {
            UpdateCamera(cam);
        }

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(Renderer.camera);

        foreach (int entity in filter)
        {
            ref CoreSystem.CGlobalTransforms transform = ref transforms.Get(entity);
            float distance = ManhattanDistance(Renderer.camera.position, transform.Position);
            if (distance < 1)
            {
                DrawModelEx(Renderer.model[0], transform.Position, transform.Axis, transform.Rotation, transform.Scale, WHITE);
            }
            else if (distance < 10 * lodScale)
            {
                DrawModelEx(Renderer.model[1], transform.Position, transform.Axis, transform.Rotation, transform.Scale, WHITE);
            }
            else if (distance < 50 * lodScale)
            {
                DrawModelEx(Renderer.model[3], transform.Position, transform.Axis, transform.Rotation, transform.Scale, WHITE);
            }
            else if (distance < 100 * lodScale)
            {
                DrawModelEx(Renderer.model[5], transform.Position, transform.Axis, transform.Rotation, transform.Scale, WHITE);
            }
            else if (distance < 200 * lodScale)
            {
                DrawCubeTexture(Renderer.texture[3], transform.Position, 1.0f, 1.0f, 1.0f, WHITE);    
            }

        }

        //DrawGrid(100, 1.0f);

        EndMode3D();

        DrawFPS(10, 10);

        EndDrawing();

        if(GetFPS() < 55){lodScale *= 0.99f;}
        if(GetFPS() > 65){lodScale *= 1.01f;}
    }

    private static float ManhattanDistance(Vector3 a, Vector3 b)
    {
        return MathF.Abs(a.X - b.X) + MathF.Abs(a.Y - b.Y) + MathF.Abs(a.Z - b.Z);
    }

    private static float Angle(Vector3 a, Vector3 b)
    {
        Vector2 aa = new Vector2(a.X, a.Z);
        Vector2 bb = new Vector2(b.X, b.Z);
        return MathF.Acos(Vector2.Dot(aa, bb));
    }
}

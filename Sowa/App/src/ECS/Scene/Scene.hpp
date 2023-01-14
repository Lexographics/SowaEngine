#ifndef _E_SCENE_HPP__
#define _E_SCENE_HPP__

#pragma once

#include "ECS/Components/Camera2D/Camera2D.hpp"
#include "ECS/Components/Transform2D/Transform2D.hpp"
#include "ECS/Entity/Entity.hpp"
#include "Resource/ResourceManager.hpp"
#include "Utils/Math.hpp"
#include "entt/entt.hpp"

#include "stlpch.hpp"

class b2World;
namespace Sowa {
class Scene {
  public:
	Scene();
	~Scene();

	Entity Create(const std::string &name, EntityID id = 0);
	void Destroy(Sowa::Entity &entity);

	entt::registry m_Registry;

	bool Save();
	bool SaveToFile(const char *file);
	bool LoadFromFile(const char *file);
	std::filesystem::path path;

	Sowa::Entity GetEntityByID(uint32_t id);

	void StartScene();
	void StopScene();

	// Copies all entities of src to dst
	static void CopyScene(Scene &src, Scene &dst);

	void CopyEntity(Sowa::Entity entity);
	void AddCopiedEntity(Sowa::Entity entity);
	void ClearCopiedEntities();
	Sowa::Entity PasteCopiedEntity();
	std::vector<Sowa::Entity> PasteCopiedEntities();
	uint32_t GetCopiedEntityCount();

	Vec2 &Gravity() { return m_Gravity; }
	// std::shared_ptr<b2World> PhysicsWorld2D() { return m_pPhysicsWorld2D; }

	Sowa::Component::Camera2D &CurrentCamera2D() { return m_SceneCamera2D.camera2d; };
	Sowa::Component::Transform2D &CurrentCameraTransform2D() { return m_SceneCamera2D.transform2d; }

	template <typename T>
	ResourceManager<T> &GetResourceManager() {
		static ResourceManager<T> manager;
		return manager;
	}

	template <typename T>
	void ClearResourceManager() {
		auto &resources = GetResourceManager<T>();
		resources.GetResources().clear();
	}

  private:
	/**
	 * @brief Registry that holds copied entities
	 */
	entt::registry m_CopyRegistry;

	Vec2 m_Gravity{0.f, -10.f};
	// std::shared_ptr<b2World> m_pPhysicsWorld2D;

	struct {
		Sowa::Component::Camera2D camera2d;
		Sowa::Component::Transform2D transform2d;
	} m_SceneCamera2D{};
	struct {
		Sowa::Component::Camera2D camera2d;
		Sowa::Component::Transform2D transform2d;
	} m_SceneOldCamera2D{};
};
} // namespace Sowa

#endif
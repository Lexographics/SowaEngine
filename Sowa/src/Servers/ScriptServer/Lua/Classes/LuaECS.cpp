#include "Servers/ScriptServer/LuaScriptServer.hpp"

#include "ECS/Components/Components.hpp"

namespace Sowa {

class ComponentBaseHandle {
  protected:
	Sowa::Entity _handle;
};

template <typename T>
class ComponentHandle : public T, public ComponentBaseHandle {
  public:
	template <typename Val, auto Member>
	Val get_item() {
		T &comp = _handle.GetComponent<T>();
		Val &val = comp.*Member;
		return val;
	}

	template <typename Val, auto Member>
	void set_item(const Val &val) {
		T &comp = _handle.GetComponent<T>();
		Val &_val = comp.*Member;
		_val = val;
	}

	ComponentHandle(Sowa::Entity &handle) {
		_handle = handle;
	}
};

void LuaScriptServer::RegisterECS() {
	_pState->new_usertype<Scene>(
		"Scene",

		"new", sol::no_constructor,

		"create",
		sol::overload(
			&Scene::Create,
			[](Sowa::Scene &self, const std::string &name) -> Sowa::Entity { return self.Create(name, 0U); }),

		"save", &Scene::Save,
		"save_to_file", &Scene::SaveToFile);

	_pState->new_enum(
		"Component",

		"Transform2D", Component::Component_Transform2D,
		"Sprite2D", Component::Component_Sprite2D,
		"Camera2D", Component::Component_Camera2D);

	_pState->new_usertype<ComponentHandle<Component::Transform2D>>(
		"Transform2D",

		"new", sol::no_constructor,

		"position",
		sol::property(
			&ComponentHandle<Component::Transform2D>::
				get_item<Sowa::Vec2, &ComponentHandle<Component::Transform2D>::m_Position>,
			&ComponentHandle<Component::Transform2D>::
				set_item<Sowa::Vec2, &ComponentHandle<Component::Transform2D>::m_Position>),

		"scale",
		sol::property(
			&ComponentHandle<Component::Transform2D>::
				get_item<Sowa::Vec2, &ComponentHandle<Component::Transform2D>::m_Scale>,
			&ComponentHandle<Component::Transform2D>::
				set_item<Sowa::Vec2, &ComponentHandle<Component::Transform2D>::m_Scale>),

		"rotation",
		sol::property(
			&ComponentHandle<Component::Transform2D>::
				get_item<float, &ComponentHandle<Component::Transform2D>::m_Rotation>,
			&ComponentHandle<Component::Transform2D>::
				set_item<float, &ComponentHandle<Component::Transform2D>::m_Rotation>));

	_pState->new_usertype<ComponentHandle<Component::Sprite2D>>(
		"Sprite2D",

		"new", sol::no_constructor,

		"texture",
		sol::property(
			&ComponentHandle<Component::Sprite2D>::
				get_item<Reference<ImageTexture>, &ComponentHandle<Component::Sprite2D>::_texture>,
			&ComponentHandle<Component::Sprite2D>::
				set_item<Reference<ImageTexture>, &ComponentHandle<Component::Sprite2D>::_texture>)
		);

	_pState->new_usertype<ComponentHandle<Component::Camera2D>>(
		"Camera2D",

		"new", sol::no_constructor,

		"current",
		sol::property(
			&ComponentHandle<Component::Camera2D>::
				get_item<bool, &ComponentHandle<Component::Camera2D>::_Current>,
			&ComponentHandle<Component::Camera2D>::
				set_item<bool, &ComponentHandle<Component::Camera2D>::_Current>),

		"rotatable",
		sol::property(
			&ComponentHandle<Component::Camera2D>::
				get_item<bool, &ComponentHandle<Component::Camera2D>::_Rotatable>,
			&ComponentHandle<Component::Camera2D>::
				set_item<bool, &ComponentHandle<Component::Camera2D>::_Rotatable>),

		"zoom",
		sol::property(
			&ComponentHandle<Component::Camera2D>::
				get_item<float, &ComponentHandle<Component::Camera2D>::_Zoom>,
			&ComponentHandle<Component::Camera2D>::
				set_item<float, &ComponentHandle<Component::Camera2D>::_Zoom>),

		"center",
		sol::property(
			&ComponentHandle<Component::Camera2D>::
				get_item<Sowa::Vec2, &ComponentHandle<Component::Camera2D>::_Center>,
			&ComponentHandle<Component::Camera2D>::
				set_item<Sowa::Vec2, &ComponentHandle<Component::Camera2D>::_Center>));

	_pState->new_usertype<Entity>(
		"Entity",

		"valid", &Entity::IsValid,
		"name",
		sol::property(
			[](Entity &self) -> const std::string & { return self.GetComponent<Sowa::Component::Common>().Name(); },
			[](Entity &self, const std::string &name) { self.GetComponent<Sowa::Component::Common>().Name() = name; }),

		"has_component", [](Entity &self, int component) -> bool {
			if (!self.IsValid())
				return false;

			if (component == Component::Component_Transform2D) {
				return self.HasComponent<Component::Transform2D>();
			}
			else if (component == Component::Component_Sprite2D) {
				return self.HasComponent<Component::Sprite2D>();
			}
			else if (component == Component::Component_Camera2D) {
				return self.HasComponent<Component::Camera2D>();
			}
			return false; },

		"add_component", [](Entity &self, int component, sol::this_state L) -> sol::variadic_results {
			if (!self.IsValid())
				return false;

			sol::variadic_results res;

			if (component == Component::Component_Transform2D) {
				if(!self.HasComponent<Component::Transform2D>()) {
					auto& comp = self.AddComponent<Component::Transform2D>();
					res.push_back({L, sol::in_place_type<Component::Transform2D>, comp});
				}
			}
			else if (component == Component::Component_Sprite2D) {
				if(!self.HasComponent<Component::Sprite2D>()) {
					auto& comp = self.AddComponent<Component::Sprite2D>();
					res.push_back({L, sol::in_place_type<Component::Sprite2D>, comp});
				}
			}
			else if (component == Component::Component_Camera2D) {
				if(!self.HasComponent<Component::Camera2D>()) {
					auto& comp = self.AddComponent<Component::Camera2D>();
					res.push_back({L, sol::in_place_type<Component::Camera2D>, comp});
				}
			}

			if(res.size() == 0)
				res.push_back(sol::nil);


			return res; },

		"remove_component", [](Entity &self, int component) -> bool {
			if (!self.IsValid())
				return false;

			if (component == Component::Component_Transform2D) {
				if(!self.HasComponent<Component::Transform2D>())
					return false;
				else {
					self.RemoveComponent<Component::Transform2D>();
					return true;
				}
			}
			else if (component == Component::Component_Sprite2D) {
				if(!self.HasComponent<Component::Sprite2D>())
					return false;
				else {
					self.RemoveComponent<Component::Sprite2D>();
					return true;
				}
			} else if (component == Component::Component_Camera2D) {
				if (!self.HasComponent<Component::Camera2D>())
					return false;
				else {
					self.RemoveComponent<Component::Camera2D>();
					return true;
				}
			}

			return false; },

		"get_component", [](Entity &self, int component, sol::this_state L) -> sol::variadic_results {
			if (!self.IsValid())
				return false;

			sol::variadic_results res;

			if (component == Component::Component_Transform2D) {
				if(self.HasComponent<Component::Transform2D>()) {
					ComponentHandle<Component::Transform2D> comp(self);

					res.push_back( { L, sol::in_place_type<ComponentHandle<Component::Transform2D>>, comp } );
				}
			}
			else if (component == Component::Component_Sprite2D) {
				if(self.HasComponent<Component::Sprite2D>()) {
					ComponentHandle<Component::Sprite2D> comp(self);

					res.push_back( { L, sol::in_place_type<ComponentHandle<Component::Sprite2D>>, comp } );
				}
			} else if (component == Component::Component_Camera2D) {
				if (self.HasComponent<Component::Camera2D>()) {
					ComponentHandle<Component::Camera2D> comp(self);

					res.push_back({L, sol::in_place_type<ComponentHandle<Component::Camera2D>>, comp});
				}
			}

			if(res.size() == 0)
				res.push_back(sol::nil);

			return res; },

		"new", sol::no_constructor);
}
} // namespace Sowa
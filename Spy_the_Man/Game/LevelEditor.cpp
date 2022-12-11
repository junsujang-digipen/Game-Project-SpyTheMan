#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <magic_enum.hpp>
#include <filesystem>
#include "LevelEditor.h"


#include "../Engine/Engine.h"
#include "../Engine/World.h"
#include "../Engine/IOManager/ObjectData.h"
#include "../Game/SkillType.h"
LevelEditor::LevelEditor()
{
	editAbleEntity.push_back(EntityType::Player);
	editAbleEntity.push_back(EntityType::Goal);
	editAbleEntity.push_back(EntityType::Hide_Obj);
	editAbleEntity.push_back(EntityType::Key);
	editAbleEntity.push_back(EntityType::Stair);
	editAbleEntity.push_back(EntityType::Clothes);
	editAbleEntity.push_back(EntityType::Lazer);
	editAbleEntity.push_back(EntityType::Worker_NPC);
	editAbleEntity.push_back(EntityType::Enemy_NPC);
	editAbleEntity.push_back(EntityType::Wall);
	editAbleEntity.push_back(EntityType::SideDoor);
	editAbleEntity.push_back(EntityType::Vent_obj);

	disguiseList.push_back(new char[]{"NOTHING"});
	disguiseList.push_back(new char[] {"Worker"});
	disguiseList.push_back(new char[] {"Guard"});

	SideList.push_back(new char[] {"NOTHING"});
	SideList.push_back(new char[] {"WALL"});
	SideList.push_back(new char[] {"DOOR"});
	
	tempTarget = new char[16]{};
	tempObjName = new char[16]{};
}

LevelEditor::~LevelEditor()
{
	for(auto t : disguiseList)
	{
		delete[] t;
	}
	for (auto t : SideList)
	{
		delete[] t;
	}
	disguiseList.clear();
	SideList.clear();
	delete[] tempTarget;
	delete[] tempObjName;
}

void LevelEditor::Load()
{
	leveldata = new PM::LevelData("Test");
	LevelName = new char[NAMEMAX]{};
	LoadIMGUI();
	bindingPos = glm::dvec2{ leveldata->GetChunkSize().x * ROOMXSIZE / 2., leveldata->GetChunkSize().y * ROOMYSIZE / 2. };
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 50);
	Engine::GetLevelIOManager().SetChapter(-1, -1);
}

void LevelEditor::Update(double dt)
{
	//state = EditorState::ObjControl;
	if (ImGui::IsAnyWindowFocused() == false && ImGui::IsAnyWindowHovered() == false)
	{
		switch (state)
		{
		case EditorState::ObjControl:
			ObjControlUpdate();
			break;

		case EditorState::RoomControl:
			RoomControlUpdate();
			break;
		}

		Engine::GetCamera().unBinding(bindingPos);
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Left))
		{
			bindingPos.x -= dt * ROOMXSIZE * 3.;
		}
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Right))
		{
			bindingPos.x += dt * ROOMXSIZE * 3.;
		}
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Up))
		{
			bindingPos.y += dt * ROOMYSIZE * 3.;
		}
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Down))
		{
			bindingPos.y -= dt * ROOMYSIZE * 3.;
		}

		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Num1))
		{
			state = EditorState::LevelSetting;
		}
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Num2))
		{
			state = EditorState::RoomControl;
		}
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Num3))
		{
			state = EditorState::ObjControl;
		}
		if (Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::Num4))
		{
			state = EditorState::DestroyObj;
		}
	}
	UpdateIMGUI(dt);
}

void LevelEditor::Draw()
{
	Engine::GetWindow().SetBackground(1.f, 1.f, 1.f, 1.f);
	Engine::GetWindow().Clear();
	math::TransformMatrix cam;
	cam.matrix = PM::getCameraMat();
	glm::ivec2 chunkSize = leveldata->GetChunkSize();
	glm::ivec2 buildingSize = {leveldata->GetChunkSize().x * ROOMXSIZE ,  leveldata->GetChunkSize().y * ROOMYSIZE };
	PM::push_settings();
	PM::apply_matrix(cam.matrix);
	//PM::set_fill_color(PM::Room::BUILDINGCOLOR.r, PM::Room::BUILDINGCOLOR.g, PM::Room::BUILDINGCOLOR.b, 255);
	//PM::draw_rect(0, 0, buildingSize.x, buildingSize.y);

	PM::set_line_color(0, 0, 0);
	PM::set_line_width(5);
	for(int i = 0; i <= chunkSize.x;++i)
	{
		PM::draw_line(ROOMXSIZE * i, 0, ROOMXSIZE * i, buildingSize.y);
	}
	for (int i = 0; i <= chunkSize.y; ++i)
	{
		PM::draw_line(0,ROOMYSIZE * i, buildingSize.x, ROOMYSIZE * i);
	}
	

		DrawRoom(cam);
	
	PM::pop_settings();
	for(auto obj : leveldata->GetobjData())
	{
		obj->Draw(cam);
	}
	glm::ivec2 mouseWPos;
	mouseWPos.x = static_cast<int>(Engine::GetInput().GetWorldMousePos().x);
	mouseWPos.y = (static_cast<int>(Engine::GetInput().GetWorldMousePos().y) / ROOMYSIZE) * ROOMYSIZE + WALLTHICKNESS;
	
	Engine::GetFont().Draw("X: " + to_string(mouseWPos.x) + " ___ Y:" + to_string(mouseWPos.y), static_cast<float>(mouseWPos.x), static_cast<float>(mouseWPos.y - 55),1,true);


	switch (state)
	{
	case EditorState::LevelSetting: break;
	case EditorState::RoomControl: break;
	case EditorState::ObjControl: break;
	case EditorState::DestroyObj:
		{
		for(auto obj : leveldata->GetobjData())
		{
			glm::vec2 p = obj->GetPosition();
			PM::push_settings();
			PM::apply_matrix(cam.matrix);
			PM::set_fill_color(255, 0, 0, 150);
			PM::draw_rect(p.x, p.y, 100, 100);
			PM::pop_settings();
		}
			
		}
		break;
	//case EditorState::Count: break;
	default: break;
	}


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LevelEditor::Unload()
{
	
	UnLoadIMGUI();
	SaveData("PreviousData");
	leveldata->Clear();
	delete[] LevelName;
	delete leveldata;
}

void LevelEditor::LoadIMGUI()
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Setup Platform/Renderer bindings
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 330";
	ImGui_ImplGlfw_InitForOpenGL(Engine::GetWindow().getWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}
void LevelEditor::UpdateIMGUI(double)
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if(ImGui::BeginMenu("Select Mode"))
	{for(int i = 0; i < static_cast<int>(EditorState::Count);++i)
	{
		
		if (ImGui::Selectable(magic_enum::enum_name(static_cast<EditorState>(i)).data()))
			ChangeState(static_cast<EditorState>(i));
	}
	
	ImGui::EndMenu();
	}
	switch (state) {

	case EditorState::LevelSetting: 
		IMGUILevelSettingUpdate();
		break;
	case EditorState::RoomControl:
		IMGUIRoomControlUpdate();
		break;
	case EditorState::ObjControl: 
		IMGUIObjControlUpdate();
		break;
	case EditorState::DestroyObj:
		IMGUIDestroyObjUpdate();
		break;
	default: break;
	}
}

void LevelEditor::UnLoadIMGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void LevelEditor::SetChunkSize(glm::ivec2 size)
{
	if (leveldata == nullptr) // editor doesn't work
	{
		return;
	}
	leveldata->SetChunkSize(size);
}

void LevelEditor::LoadData(std::string name)
{
	PM::LevelData* temp = Engine::GetLevelIOManager().ReadLevel(name,"Level/");
	if(temp == nullptr) // data doesn't exist
	{
		return;
	}
	if (leveldata != nullptr) // remove old data
	{
		delete leveldata;
	}
	leveldata = temp;
	
}

void LevelEditor::SaveData(std::string name)
{
	if(leveldata == nullptr) // editor doesn't work
	{
		Engine::GetLogger().LogError("level data == nullptr");
		return;
	}
	else if(name.empty())
	{
		Engine::GetLogger().LogError("name is empty");
		return;
	}
	leveldata->SetName(name);
	Engine::GetLevelIOManager().WriteLevel(leveldata);
	
}
void LevelEditor::RoomControlUpdate()
{
	if(Engine::GetInput().LeftMouseIsClicked() == true)
	{
		glm::ivec2 pos;
		pos.x = (static_cast<int>(Engine::GetInput().GetWorldMousePos().x) / ROOMXSIZE) * ROOMXSIZE;
		pos.y = (static_cast<int>(Engine::GetInput().GetWorldMousePos().y) / ROOMYSIZE) * ROOMYSIZE;
		if (IsInsideChunk(Engine::GetInput().GetWorldMousePos()) == true)
		{
			if (selectFirst == false)
			{
				if(IsInsideRoom(pos))
				{
					Engine::GetLogger().LogDebug("room already exist");
					return;
				}
				selectFirstInfo = pos;
			}
			else
			{
				if(IsInsideRoom(pos,selectFirstInfo))
				{
					Engine::GetLogger().LogDebug("room already exist");
					selectFirst = false;
					return;
				}
				makeRoom(selectFirstInfo, pos);
			}

			selectFirst = !selectFirst;
		}
	}
	else if (Engine::GetInput().RightMouseIsClicked() == true)
	{
		if(selectFirst == true)
		{
			selectFirst = false;
			return;
		}
		glm::dvec2 pos = Engine::GetInput().GetWorldMousePos();
		pos.x = static_cast<double>((static_cast<int>(pos.x) / ROOMXSIZE) * ROOMXSIZE);
		pos.y = static_cast<double>((static_cast<int>(pos.y) / ROOMYSIZE) * ROOMYSIZE);
		destroyRoom(pos);
	}
}

void LevelEditor::ObjControlUpdate()
{
	if (Engine::GetInput().RightMouseIsClicked())
	{
		CheckSameName();
		auto& data = leveldata->GetobjData();
		//tempObjectName = name;
		switch (selectType) {

		case EntityType::Wall:
			SetWall(isleft);
			break;
		case EntityType::SideDoor:

			SetDoor(isleft);
			break;
		case EntityType::Vent_obj:

			if (selectFirst)
			{
				xsize = tempSecondObjPos.x - tempObjectPos.x;
				if (xsize < 0)
				{
					data.push_back(new PM::VentData(tempObjectName, { tempSecondObjPos.x,GetCNum({tempSecondObjPos.x,tempSecondObjPos.y}).y }, -xsize));
				}
				else
				{
					data.push_back(new PM::VentData(tempObjectName, { tempObjectPos.x,GetCNum({tempSecondObjPos.x,tempSecondObjPos.y}).y }, xsize));
				}
				selectFirst = false;
			}
			else
			{
				tempSecondObjPos.x = tempObjectPos.x;
				tempSecondObjPos.y = tempObjectPos.y;
				selectFirst = true;
			}

			break;
		case EntityType::Stair:
		{
			if (selectFirst)
			{
				TargetName = tempObjectName + "Target";
				bool down{ false };
				if (tempSecondObjPos.y > tempObjectPos.y)
				{
					down = true;
				}
				data.push_back(new PM::StairData(tempObjectName, { tempSecondObjPos.x, tempSecondObjPos.y }, TargetName, down));
				data.push_back(new PM::StairData(TargetName, { tempSecondObjPos.x, tempObjectPos.y }, tempObjectName, !down));
				selectFirst = false;
			}
			else
			{
				tempSecondObjPos.x = tempObjectPos.x;
				tempSecondObjPos.y = tempObjectPos.y;
				selectFirst = true;
			}
		}
		break;

		case EntityType::Player:

			[[fallthrough]];
		case EntityType::Goal:

			[[fallthrough]];
		case EntityType::Hide_Obj:
		{
			data.push_back(new PM::ObjData(tempObjectName, tempObjectPos, selectType));
		}
		break;

		case EntityType::Key:
		{
			if (TargetName.empty())
			{
				Engine::GetLogger().LogError("Must Set Target Name");
				return;
			}
			data.push_back(new PM::KeyData(tempObjectName, tempObjectPos, TargetName));
		}break;
		case EntityType::Clothes:
			data.push_back(new PM::ClothesData(tempObjectName, tempObjectPos, D_list));
			break;
		case EntityType::Lazer:
		{
			if (isMove == false)
			{
				tempObjectPos.y -= WALLTHICKNESS;
				data.push_back(new PM::LazerData(tempObjectName, tempObjectPos, onofftime, tempSecondObjPos));
			}
			else
			{
				if (selectFirst)
				{
					float min = std::min(tempSecondObjPos.x, tempObjectPos.x);
					float max = std::max(tempSecondObjPos.x, tempObjectPos.x);
					float w = 0;
					if(min >= tempSecondObjPos.x)
					{
						w = 1.f;
					}
					else
					{
						w = -1.f;
					}
					tempSecondObjPos.y -= WALLTHICKNESS;
					data.push_back(new  PM::LazerData(tempObjectName, { tempSecondObjPos.x, tempSecondObjPos.y },onofftime,{ min,max,tempSecondObjPos.z,w}));
					selectFirst = false;
				}
				else
				{
					tempSecondObjPos.x = tempObjectPos.x;
					tempSecondObjPos.y = tempObjectPos.y;
					selectFirst = true;
				}
			}
		}
		    break;
		case EntityType::Worker_NPC:
			[[fallthrough]];
		case EntityType::Enemy_NPC:
			if (selectFirst)
			{
				data.push_back(new PM::NPCData(tempObjectName, { tempSecondObjPos.x,tempSecondObjPos.y }, selectType,
					{ tempSecondObjPos.x,tempSecondObjPos.y }, tempObjectPos));
				selectFirst = false;
			}
			else
			{
				tempSecondObjPos.x = tempObjectPos.x;
				tempSecondObjPos.y = tempObjectPos.y;
				selectFirst = true;
			}
			break;
			//case EntityType::Switch: break;
		default: return;
		}
	}
}



void LevelEditor::IMGUILevelSettingUpdate()
{					   
	
	if (ImGui::Button("Save"))
	{
		SaveData(LevelName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		loadbuttonOn = !loadbuttonOn;
		PlayButtonOn = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Play"))
	{
		PlayButtonOn = !PlayButtonOn;
		loadbuttonOn = false;
	}
	if (loadbuttonOn)
	{
		if (ImGui::BeginTabBar("Load"))
		{
			ImGui::Text("Sellect File");
			for (const auto& file : filesystem::directory_iterator("Level"))
			{
	
					std::string name = relative(file).filename().string();
					auto a = name.c_str();
					if(ImGui::Selectable(a))
					{
						LoadData(name);
					}
			}
		}
	}
	if(PlayButtonOn)
	{
		if (ImGui::BeginTabBar("Load"))
		{
			ImGui::Text("Sellect File");
			for (const auto& file : filesystem::directory_iterator("Level"))
			{

				std::string name = relative(file).filename().string();
				auto a = name.c_str();
				if (ImGui::Selectable(a))
				{
					Engine::GetGameStateManager().SetLevelState(name,"Level/");
				}
			}
		}
	}
	ImGui::InputText("LevelName",LevelName, NAMEMAX - 1);

	glm::ivec2 size_ = leveldata->GetChunkSize();

	ImGui::InputInt2("ChunkSize", &size_.x);
	if(size_ != leveldata->GetChunkSize())
	{
		SetChunkSize(size_);
	}

	
	int count = static_cast<int>(SkillType::Null);
	for(int i = 0; i < count; i++)
	{
		 bool a = leveldata->GetSkillset(static_cast<SkillType>(i));
		 SkillType type = static_cast<SkillType>(i);
		 auto name = magic_enum::enum_name(type);
		 ImGui::Checkbox(name.data(), &a); // enum cast?
		leveldata->SetSkillset(static_cast<SkillType>(i), a);
	}
}

void LevelEditor::IMGUIRoomControlUpdate()
{
	ImGui::Text("ROOM Control Mode");
	ImGui::Text("Left Click Twice: Make Room");
	ImGui::Text("Left -> Right Click: Cancel Make Room");
	ImGui::Text("Right Click: Destroy Room");	
	//if (ImGui::BeginTabBar("ROOM_LIST"))
	{
		
		auto& data = leveldata->GetRoomData();
		for(int i = 0; i <data.size(); ++i)
		{
			//if(ImGui::BeginTabItem(data[i]->Name.c_str()))
			{

				ImGui::Text(data[i]->Name.c_str());

				auto& objData = leveldata->GetobjData();
				bool left = false;
				bool right = false;
				for (auto& obj : objData)
				{
					if (data[i]->Left == obj->GetName()) { left = true; }
					if (data[i]->Right == obj->GetName()) { right = true; }
				}
				

			
				ImGui::Text("Left: ");
				ImGui::SameLine();
				ImGui::Text(data[i]->Left.c_str());
				if(left == true)
				{
					ImGui::Text("L Find ");
				}
				else
				{
					ImGui::Text("X ");
				}
				ImGui::Text("Right: ");
				ImGui::SameLine();
				ImGui::Text(data[i]->Right.c_str());
				if (right == true)
				{
					ImGui::Text("R Find ");
				}
				else
				{
					ImGui::Text("X ");
				}
				ImGui::Text("----------------------");
			}
		}
		//ImGui::EndTabBar();
	}
}

void LevelEditor::IMGUIObjControlUpdate()
{
	ImGui::Text("OBJ Control Mode");
	
	for(auto type : editAbleEntity)
	{
		auto name = magic_enum::enum_name(type);
		if(ImGui::Selectable(name.data()))
		{
			selectType = type;
			selectFirst = false;
		}
	}

	if(selectType != EntityType::Count)
	{
		glm::dvec2 pos = Engine::GetInput().GetWorldMousePos();
		pos.y = (static_cast<int>(pos.y) / ROOMYSIZE) * ROOMYSIZE + WALLTHICKNESS;
		tempObjectPos = pos;
	
		
		ImGui::InputText("Name", tempObjName, 15);
		std::string str{ tempObjName };
		tempObjectName = str;
		switch (selectType) {

		case EntityType::SideDoor:
			ImGui::Checkbox("IsLock", &isLock);
			[[fallthrough]];
		case EntityType::Wall: 
			ImGui::Checkbox("IsLeft", &isleft);

			break;
		case EntityType::Vent_obj: 
			ImGui::Text("Press First: first pos");
			ImGui::Text("Press Second: second pos ");
			break;
		case EntityType::Stair:
			ImGui::Text("Press First: first obj x and floor");
			ImGui::Text("Press Second: second obj floor ");
			break;
		case EntityType::Key:
		{
		
			ImGui::InputText("Target", tempTarget,  15);
			std::string str = tempTarget;
			TargetName = str;
		}
			break;
		case EntityType::Clothes:
			static int num;
			
			ImGui::ListBox("Disguise", &num, {disguiseList.data()}, static_cast<int>(DisguiseList::Conut));
			D_list = static_cast<DisguiseList>(num);
			break;
			case EntityType::Lazer:

				ImGui::InputFloat("OnOffTime", &onofftime);
				ImGui::InputFloat("Speed", &tempSecondObjPos.z);
				ImGui::Checkbox("IsMove", &isMove);
				break;

		case EntityType::Worker_NPC: [[fallthrough]];
		case EntityType::Enemy_NPC:
		{
			ImGui::Text("Press First: obj pos and first move point");
			ImGui::Text("Press Second: second move point ");
			//ImGui::InputFloat2("Point1", &tempSecondObjPos.x);
			//ImGui::InputFloat2("Point2", &tempSecondObjPos.z);
		}
		break;
		//case EntityType::Switch: break;
		default: break;
	}


		
	}
}

void LevelEditor::IMGUIDestroyObjUpdate()
{
	ImGui::Text("Destroy Obj Mode");
	ImGui::Text("Left Click: Destroy obj");
	ImGui::Text("Click the red block");
	ImGui::Text("you must destroy stair twice");
	if(Engine::GetInput().LeftMouseIsReleased())
	{
		DestroyObj(Engine::GetInput().GetWorldMousePos());
	}
}




bool LevelEditor::IsInsideChunk(glm::vec2 pos)
{
	glm::ivec2 buildingSize = { leveldata->GetChunkSize().x * ROOMXSIZE ,  leveldata->GetChunkSize().y * ROOMYSIZE };
	
	return pos.x >= 0 && pos.x < buildingSize.x &&
		pos.y >= 0 && pos.y < buildingSize.y;
}
bool LevelEditor::IsInsideRoom(glm::vec2 pos)
{
	for(const auto room : leveldata->GetRoomData())
	{
		if (room->position.x <= pos.x && room->position.x + room->size.x > pos.x)
		{
			if (room->position.y <= pos.y && room->position.y + ROOMYSIZE > pos.y)
			{
				return true;
			}
		}
	}
	return false;
}

PM::RoomData* LevelEditor::GetRoom(glm::vec2 pos)
{
	for (auto room : leveldata->GetRoomData())
	{
		if (room->position.x <= pos.x && room->position.x + room->size.x > pos.x)
		{
			if (room->position.y <= pos.y && room->position.y + ROOMYSIZE > pos.y)
			{
				return room;
			}
		}
	}
	return nullptr;
}


bool LevelEditor::IsInsideRoom(glm::vec2 pos1, glm::vec2 pos2)
{
	glm::vec2 min = { std::min(pos1.x,pos2.x), pos1.y };
	glm::vec2 max = { std::max(pos1.x,pos2.x),pos1.y };
	for (const auto room : leveldata->GetRoomData())
	{
		if (room->position.y <= pos1.y && room->position.y + ROOMYSIZE > pos1.y)
		{
			if (room->position.x <= max.x && room->position.x + room->size.x > min.x)
			{
				return true;
			}
		}
	}
	return false;
}

void LevelEditor::makeRoom(glm::vec2 pos1, glm::vec2 pos2)
{
	glm::vec2 min = { std::min(pos1.x,pos2.x), pos1.y };
	glm::vec2 max = { std::max(pos1.x,pos2.x),pos1.y };
	PM::RoomData* temp = new PM::RoomData();
	temp->position = min;
	temp->size.x = ROOMXSIZE + max.x - min.x;
	temp->size.y = ROOMYSIZE;
	temp->Name = to_string(GetCNum(min).y) + "floor" + to_string(GetCNum(min).x) + "to" + to_string(GetCNum(max).x);
	
	leveldata->GetRoomData().push_back(temp);
}

void LevelEditor::destroyRoom(glm::vec2 pos)
{
	std::vector<PM::RoomData*>& data = leveldata->GetRoomData();
	for (int i = 0; i < data.size(); ++i)
	{
		auto room = data[i];
		if (room->position.x <= pos.x && room->position.x + room->size.x > pos.x)
		{
			if (room->position.y <= pos.y && room->position.y + ROOMYSIZE > pos.y)
			{
				delete room;
				data.erase(data.begin() + i);
				return;
			}
		}
	}
}

glm::ivec2 LevelEditor::GetCNum(glm::vec2 pos)
{
	return { static_cast<int>(pos.x) / ROOMXSIZE, static_cast<int>(pos.y) / ROOMYSIZE };
}

void LevelEditor::DrawRoom(math::TransformMatrix& mat)
{
	for(auto a : leveldata->GetRoomData())
	{
		PM::push_settings();
		PM::apply_matrix(mat.matrix);
		glm::ivec2 cp = GetCNum(a->position) + 1;
		PM::no_outline();
		PM::set_fill_color(50,static_cast<double>(cp.x) / leveldata->GetChunkSize().x * 255, static_cast<double>(cp.y) / leveldata->GetChunkSize().y * 255,100);
		PM::draw_rect(a->position.x, a->position.y, a->size.x, a->size.y);
		PM::pop_settings();
	}

	if(selectFirst == true && state == EditorState::RoomControl)
	{
		PM::push_settings();
		PM::apply_matrix(mat.matrix);

		PM::set_line_width(5);
		PM::draw_line(selectFirstInfo.x + ROOMXSIZE/2, selectFirstInfo.y + ROOMYSIZE / 2, Engine::GetInput().GetWorldMousePos().x, selectFirstInfo.y + ROOMYSIZE / 2);
		PM::pop_settings();
	}
}

void LevelEditor::CheckSameName()
{
	if(tempObjectName.empty())
	{
		tempObjectName = to_string(static_cast<int>(Engine::GetRandomNum(0, 100000)));
	}
	for(auto& data : leveldata->GetobjData())
	{
		if(data->GetName() == tempObjectName)
		{
			tempObjectName = to_string(static_cast<int>(Engine::GetRandomNum(0, 100000)));
			CheckSameName();
		}
	}
	return;
}


void LevelEditor::SetDoor(bool isLeft)
{
	if (IsInsideChunk(tempObjectPos) == false) { return; }
	if (IsInsideRoom(tempObjectPos) == false) { return; }

	glm::ivec2 chunkNum = GetCNum(tempObjectPos);
	glm::vec2 pos;
	if(isLeft)
	{
		if(chunkNum.x == 0)
		{
			Engine::GetLogger().LogDebug("Can't Set Door in first Left");
			return;
		}
		PM::RoomData* temp = GetRoom(tempObjectPos);
		pos = { temp->position.x - WALLTHICKNESS, tempObjectPos.y };
		temp->Left = tempObjectName;
		PM::RoomData* tempLeft = GetRoom(tempObjectPos - glm::vec2{ ROOMXSIZE,0 });
		
		if(tempLeft != nullptr)
		{
			tempLeft->Right = tempObjectName;
		}

	}
	else
	{
		if (chunkNum.x == leveldata->GetChunkSize().x - 1)
		{
			Engine::GetLogger().LogDebug("Can't Set Door in last Right chunk");
			return;
		}
	
		PM::RoomData* temp = GetRoom(tempObjectPos);
		pos = { temp->position.x + temp->size.x - WALLTHICKNESS, tempObjectPos.y };
		temp->Right = tempObjectName;
		PM::RoomData* tempRight = GetRoom(pos + glm::vec2{ROOMXSIZE,0});

		if (tempRight != nullptr)
		{
			tempRight->Left = tempObjectName;
		}
	}
	
	
	leveldata->GetobjData().push_back(new PM::SideDoorData(tempObjectName, pos, isLock));
	
}
void LevelEditor::SetWall(bool isLeft)
{
	if (IsInsideChunk(tempObjectPos) == false) { return; }
	if (IsInsideRoom(tempObjectPos) == false) { return; }

	glm::ivec2 chunkNum = GetCNum(tempObjectPos);
	glm::vec2 pos;
	glm::vec2 size = { 50,300 + WALLTHICKNESS };
	if (isLeft)
	{
		PM::RoomData* temp = GetRoom(tempObjectPos);
		pos = { temp->position.x - WALLTHICKNESS, tempObjectPos.y - WALLTHICKNESS };
		
		temp->Left = tempObjectName;
		PM::RoomData* tempLeft = GetRoom(tempObjectPos - glm::vec2{ ROOMXSIZE,0 });

		if (tempLeft != nullptr)
		{
			tempLeft->Right = tempObjectName;
		}
		if (chunkNum.x == 0)
		{
			pos.x += WALLTHICKNESS;
			size.x = WALLTHICKNESS;
		}
	}
	else
	{
		
		PM::RoomData* temp = GetRoom(tempObjectPos);
		temp->Right = tempObjectName;
		pos = { temp->position.x + temp->size.x - WALLTHICKNESS, tempObjectPos.y - WALLTHICKNESS };
		PM::RoomData* tempRight = GetRoom(pos + glm::vec2{ ROOMXSIZE,0 });

		if (tempRight != nullptr)
		{
			tempRight->Left = tempObjectName;
		}
		if (chunkNum.x == leveldata->GetChunkSize().x - 1)
		{
		
			size.x = WALLTHICKNESS;
		}
	}


	leveldata->GetobjData().push_back(new PM::WallData(tempObjectName, pos, size));
}

void LevelEditor::DestroyObj(glm::vec2 pos)
{
	auto& data = leveldata->GetobjData();
	for(int i = 0; i < data.size(); ++i)
	{
		auto& obj = data[i];
		{
			if (obj->GetPosition().x < pos.x && pos.x < obj->GetPosition().x + 100.f)
			{
				if (obj->GetPosition().y < pos.y && pos.y < obj->GetPosition().y + 100.f)
				{
					delete obj;
					data.erase(data.begin() + i);
					return;
				}
			}
		}
	}
}

void LevelEditor::ChangeState(EditorState state_)
{
	selectFirst = false;


	this->state = state_;
}

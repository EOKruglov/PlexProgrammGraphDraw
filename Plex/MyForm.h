#pragma once
#include "Plex.h"
namespace Plex {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		int x1, x2, y1, y2;
		bool flag;
		TChart *pFirst;
		TChart *pCatch;
		TPoint *p1, *p2, *currentPoint;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
			 Graphics ^gr;
	public:
		MyForm(void)
		{
			InitializeComponent();
			flag = false;
			gr = CreateGraphics();
			pFirst = NULL;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(983, 30);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Move";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(983, 88);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Delete";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MyForm::button2_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1140, 749);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseDoubleClick);
			this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseDown);
			this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseMove);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseUp);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void MyForm_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		flag = true;
		x1 = x2 = e->X;
		y1 = y2 = e->Y;
	}
	private: System::Void MyForm_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		if (flag)
		{
			gr->DrawLine(Pens::White, x1, y1, x2, y2);
			x2 = e->X;
			y2 = e->Y;
			gr->DrawLine(Pens::Black, x1, y1, x2, y2);
		}
	}
	private: System::Void MyForm_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		flag = false;
		if (pFirst == NULL)
		{
			p1 = new TPoint(x1, y1);
			p2 = new TPoint(x2, y2);
			pFirst = new TChart;
			pFirst->setBegin(p1);
			pFirst->setEnd(p2);
			pFirst->Show(gr);
		}
		else
		{
			if (pFirst->Find(x1, y1))
			{
				p2 = new TPoint(x2, y2);
				if (pFirst->first)
					p1 = (TPoint*)pFirst->pRes->GetBegin();
				else
					p1 = (TPoint*)pFirst->pRes->GetEnd();
				TChart *tmp = new TChart;
				tmp->setBegin(p2);
				tmp->setEnd(p1);
				if (pFirst->first)
					pFirst->pRes->setBegin(tmp);
				else
					pFirst->pRes->setEnd(tmp);
			}
			if (pFirst->Find(x2, y2))
			{
				p2 = new TPoint(x1, y1);
				if (pFirst->first)
					p1 = (TPoint*)pFirst->pRes->GetBegin();
				else
					p1 = (TPoint*)pFirst->pRes->GetEnd();
				TChart *tmp = new TChart;
				tmp->setBegin(p1);
				tmp->setEnd(p2);
				if (pFirst->first)
					pFirst->pRes->setBegin(tmp);
				else
					pFirst->pRes->setEnd(tmp);
			}
			if(!pFirst->Find(x1, y1) && !pFirst->Find(x2, y2))
				gr->DrawLine(Pens::White, x1, y1, x2, y2);
		}
	}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		pFirst->Move(gr, 10, 10);
	}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	pFirst->Hide(gr);
	pFirst = nullptr;
}
private: System::Void MyForm_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	TChart *tmp = pFirst->Catch(e->X, e->Y);
	pCatch = pFirst;
	if (tmp != nullptr) {
		if (e->Button.ToString() == "Left") {
			pCatch -> SetActive(false);
			pCatch = tmp;
			pCatch->SetActive(true);
		}
		if (e->Button.ToString() == "Right") {
			tmp->SetVisible(false);
		}
	}

	TPoint *p = pFirst->CatchPoint(e->X, e->Y);
	if (p != nullptr) {
		if (e->Button.ToString() == "Left") {
			currentPoint->SetActive(false);
			currentPoint = p;
			currentPoint->SetActive(true);
		}
		if (e->Button.ToString() == "Right") {
			p->SetVisible(false);
		}
	}

	pFirst->Hide(gr);
	pFirst->Show(gr);
}
};
}

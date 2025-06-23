using UnityEngine;

public class UiManagerMainMenu : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        SoundManager.Instance.f_AutoPlayBGM(); //���� �޴� ������ �ڵ����� ������� ���
    }

    // Update is called once per frame
    void Update()
    {

    }

    public void f_OnClickStartButton() //���� ���� ��ư Ŭ�� �� "BeginScene" ������ ��ȯ�ϴ� �޼ҵ�
    {
        FlowManager.Instance.f_OpenScene(SceneName.BeginScene); //���� ���� ��ư Ŭ�� �� "BeginScene" ������ ��ȯ
    }

    public void f_OnClickExitButton() //���� ���� ��ư Ŭ�� �� ���ø����̼��� �����ϴ� �޼ҵ�
    {
        Application.Quit(); //���ø����̼� ����
    }

}

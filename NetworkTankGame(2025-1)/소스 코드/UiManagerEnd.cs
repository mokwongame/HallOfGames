using UnityEngine;

public class UiManagerEnd : MonoBehaviour
{
    // Start is called once before the first execution of Update after the MonoBehaviour is created
    void Start()
    {
        SoundManager.Instance.f_StopAllBGM(); //��� ������� ����
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void f_OnClickExitButton() //���� ���� ��ư Ŭ�� �� ���ø����̼��� �����ϴ� �޼ҵ�
    {
        Application.Quit(); //���ø����̼� ����
    }
}

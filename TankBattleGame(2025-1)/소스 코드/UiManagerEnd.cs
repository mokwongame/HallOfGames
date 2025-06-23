using System.Collections;
using Unity.Netcode;
using Unity.Netcode.Transports.UTP;
using UnityEngine;
using UnityEngine.SceneManagement;

// ���� ���� ȭ�鿡�� UI�� �����ϴ� Ŭ����
public class UiManagerEnd : MonoBehaviour
{
    // ���� �� ȣ��: ���� �׸� ���� ���
    private void Start()
    {
        SoundManager.Instance.PlaySound(SoundType.ThemeEnd, true);  // �ݺ� ���
    }

    // '���� �޴���' ��ư Ŭ�� �� ȣ��
    public void OnMainMenuBtnClicked()
    {
        SoundManager.Instance.StopPlaySound(SoundType.ThemeEnd);  // ���� ����
        StartCoroutine(nameof(ReturnToMainMenu));                 // ���� �޴��� ���ư��� �ڷ�ƾ ����
    }

    // '���� ����' ��ư Ŭ�� �� ȣ��
    public void OnExitBtnClicked()
    {
        SoundManager.Instance.StopPlaySound(SoundType.ThemeEnd);    // ���� ����

#if UNITY_EDITOR
        UnityEditor.EditorApplication.isPlaying = false;            // ������ ���� �ߴ�
#else
        Application.Quit();  // ����� ���ø����̼� ����
#endif
    }

    // ��Ʈ��ũ ���� �� ���� �޴� ������ ��ȯ�ϴ� �ڷ�ƾ
    private IEnumerator ReturnToMainMenu()
    {
        // ���� �Ǵ� Ŭ���̾�Ʈ�� ���� ���̶�� ���� ��û
        if (NetworkManager.Singleton.IsListening)
        {
            NetworkManager.Singleton.Shutdown();  // ��Ʈ��ũ ����
        }

        // ������ ����� ������ ���
        yield return new WaitUntil(() => NetworkManager.Singleton.IsListening == false);

        // ���� �޴� �� �ε�
        SceneManager.LoadScene("BeginScene");
    }
}

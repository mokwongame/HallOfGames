using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class SkillUIController : MonoBehaviour
{
    [System.Serializable]
    public class SkillUI
    {
        public Image icon;               // ��ų ������ �̹���
        public Image cooldownOverlay;    // ��ٿ� ����ũ �̹��� (Fill Type: Filled)
        public Image lockOverlay;        // ��� ǥ�� �̹���
        public float cooldownDuration;   // �ش� ��ų ��ٿ� �ð�

        [HideInInspector] public float lastUsedTime = -999f;  // ������ ��� �ð� ����
        [HideInInspector] public bool isUnlocked = false;     // �ر� ����
    }

    public SkillUI skillZ; // Z ��ų UI
    public SkillUI skillX; // X ��ų UI
    public SkillUI skillC; // C ��ų UI

    void Start()
    {
        string scene = SceneManager.GetActiveScene().name;

        // ���� ���� ���� ��ų �ر� ���� ����
        skillZ.isUnlocked = (scene == "Stage1" || scene == "Stage2" || scene == "Stage3");
        skillX.isUnlocked = (scene == "Stage2" || scene == "Stage3");
        skillC.isUnlocked = (scene == "Stage3");

        // �ر� �ȵ� ��ų�� ��ٿ� ����ũ �� ä��� (���û���)
        if (!skillZ.isUnlocked) skillZ.cooldownOverlay.fillAmount = 1f;
        if (!skillX.isUnlocked) skillX.cooldownOverlay.fillAmount = 1f;
        if (!skillC.isUnlocked) skillC.cooldownOverlay.fillAmount = 1f;
    }

    void Update()
    {
        UpdateSkillUI(skillZ);
        UpdateSkillUI(skillX);
        UpdateSkillUI(skillC);
    }

    void UpdateSkillUI(SkillUI skill)
    {
        if (skill.isUnlocked)
        {
            skill.lockOverlay.enabled = false;
            float elapsed = Time.time - skill.lastUsedTime;
            // ��ٿ� ���� ���� (1->0���� ä����)
            float fillPercent = Mathf.Clamp01(1f - (elapsed / skill.cooldownDuration));
            skill.cooldownOverlay.fillAmount = fillPercent;
        }
        else
        {
            // ��ų ��� ���¸� ��� �̹��� ǥ��, ��ٿ� ����ũ�� �� ä��
            skill.lockOverlay.enabled = true;
            skill.cooldownOverlay.fillAmount = 1f;
        }
    }

    // ��ٿ� ���� Ʈ���� (Ű �̸����� ȣ��)
    public void TriggerCooldown(string key)
    {
        switch (key.ToUpper())
        {
            case "Z":
                if (skillZ.isUnlocked)
                    skillZ.lastUsedTime = Time.time;
                break;
            case "X":
                if (skillX.isUnlocked)
                    skillX.lastUsedTime = Time.time;
                break;
            case "C":
                if (skillC.isUnlocked)
                    skillC.lastUsedTime = Time.time;
                break;
        }
    }
}
